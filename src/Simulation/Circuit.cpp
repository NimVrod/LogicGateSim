#include "Circuit.h"
#include <algorithm>
#include <ranges>
#include <cmath>
#include <Core/Components/Include/Button.h>
#include <Core/Components/Include/ClockComponent.h>

void Circuit::addComponent(std::unique_ptr<Component> component) {
    components.push_back(std::move(component));
    draggedComponent = components.back().get();
    dragOffset = sf::Vector2f(0.f, 0.f);
    state = circuitState::DraggingComponent;
}

void Circuit::addWire(Pin *start, Pin *end) {
    if (start && end && start != end) {
        if (start->getType() == PinType::Input && end->getType() == PinType::Output) {
            std::swap(start, end); // Ensure start is Output and end is Input
        }
        if (start->getType() == PinType::Output && end->getType() == PinType::Input) {
            wires.push_back(std::make_unique<Wire>(start, end));
        }
    }
}

void Circuit::clear() {
    wires.clear();
    components.clear();
    selectedPin = nullptr;
    draggedComponent = nullptr;
    hoveredComponent = nullptr;
    state = circuitState::Idle;
    nextComponentId = 1; // Reset component ID counter
}

void Circuit::tickClocks() {
    for (auto &comp: components) {
        if (auto *clock = dynamic_cast<ClockComponent *>(comp.get())) {
            clock->tick();
        }
    }
}

void Circuit::update() {
    for (auto &wire: wires) {
        wire->update();
    }
    for (auto &comp: components) {
        comp->calculate();
    }
}

void Circuit::draw(sf::RenderWindow &window) {
    if (showGrid) {
        drawGrid(window);
    }

    for (auto &wire: wires) {
        wire->draw(window);
    }
    for (auto &comp: components) {
        comp->draw(window);
        if (drawAllPins)
            comp->drawPins(window);
        if (drawLabels)
            comp->drawLabel(window);
    }

    if (hoveredComponent) {
        hoveredComponent->drawPins(window);
        hoveredComponent->drawLabel(window);
    }

    if (selectedPin) {
        selectedPin->draw(window);
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

        sf::Vertex line[] = {
            sf::Vertex{selectedPin->getPosition(), sf::Color::Yellow},
            sf::Vertex{worldPos, sf::Color::Yellow}
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }
}

void Circuit::drawGrid(sf::RenderWindow &window) {
    sf::View view = window.getView();
    sf::Vector2f viewCenter = view.getCenter();
    sf::Vector2f viewSize = view.getSize();

    float left = viewCenter.x - viewSize.x / 2;
    float right = viewCenter.x + viewSize.x / 2;
    float top = viewCenter.y - viewSize.y / 2;
    float bottom = viewCenter.y + viewSize.y / 2;

    float startX = std::floor(left / gridSize) * gridSize;
    float startY = std::floor(top / gridSize) * gridSize;

    sf::Color gridColor(50, 50, 50);

    for (float x = startX; x <= right; x += gridSize) {
        sf::Vertex line[] = {
            sf::Vertex{sf::Vector2f(x, top), gridColor},
            sf::Vertex{sf::Vector2f(x, bottom), gridColor}
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }

    for (float y = startY; y <= bottom; y += gridSize) {
        sf::Vertex line[] = {
            sf::Vertex{sf::Vector2f(left, y), gridColor},
            sf::Vertex{sf::Vector2f(right, y), gridColor}
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }
}

sf::Vector2f Circuit::snapPosition(sf::Vector2f pos) const {
    if (!snapToGrid) return pos;
    return sf::Vector2f(
        std::round(pos.x / gridSize) * gridSize,
        std::round(pos.y / gridSize) * gridSize
    );
}

void Circuit::setDrawAllPins(bool draw) {
    drawAllPins = draw;
}

void Circuit::setDrawLabels(bool draw) {
    drawLabels = draw;
}

Pin *Circuit::getPinAt(sf::Vector2f pos) {
    for (auto &comp: components) {
        for (auto &pin: comp->getInputs()) {
            if (pin->mouseOver(pos)) return pin.get();
        }
        for (auto &pin: comp->getOutputs()) {
            if (pin->mouseOver(pos)) return pin.get();
        }
    }
    return nullptr;
}

void Circuit::handleEvent(const sf::Event &event, sf::RenderWindow &window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    if (event.is<sf::Event::MouseMoved>()) {
        hoveredComponent = nullptr;
        for (auto &component: std::ranges::reverse_view(components)) {
            Component *comp = component.get();
            if (comp->getBounds().contains(worldPos)) {
                hoveredComponent = comp;
                break;
            }
            else {
                for (auto &pin: comp->getInputs()) {
                    if (pin->mouseOver(worldPos)) hoveredComponent = comp;
                }
                for (auto &pin: comp->getOutputs()) {
                    if (pin->mouseOver(worldPos)) hoveredComponent = comp;
                }
            }
        }

        switch (state) {
            case circuitState::DraggingComponent:
                if (draggedComponent) {
                    sf::Vector2f newPos = worldPos - dragOffset;
                    draggedComponent->setPosition(snapPosition(newPos));
                }
                break;
            default:
                break;
        }
        return;
    }

    if (const auto *mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        switch (state) {
            case circuitState::Idle:
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    if (Pin *clickedPin = getPinAt(worldPos)) {
                        selectedPin = clickedPin;
                        state = circuitState::CreatingWire;
                        return;
                    }

                    for (auto &component: std::ranges::reverse_view(components)) {
                        Component *comp = component.get();
                        if (comp->getBounds().contains(worldPos)) {
                            if (auto *button = dynamic_cast<Button *>(comp)) {
                                sf::FloatRect bounds = button->getBounds();
                                float margin = bounds.size.x * 0.2f;
                                sf::FloatRect innerBounds(
                                    sf::Vector2f(bounds.position.x + margin, bounds.position.y + margin),
                                    sf::Vector2f(bounds.size.x - 2 * margin, bounds.size.y - 2 * margin)
                                );
                                if (innerBounds.contains(worldPos)) {
                                    button->toggle();
                                    return;
                                }
                            }
                            draggedComponent = comp;
                            dragOffset = worldPos - comp->getPosition();
                            state = circuitState::DraggingComponent;
                            return;
                        }
                    }
                } else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    for (auto &component: std::ranges::reverse_view(components)) {
                        Component *comp = component.get();
                        if (comp->getBounds().contains(worldPos)) {
                            contextMenuComponentId = comp->GetId();
                            return;
                        }
                    }
                }
                break;

            case circuitState::CreatingWire:
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    if (Pin *targetPin = getPinAt(worldPos); targetPin && targetPin != selectedPin) {
                        addWire(selectedPin, targetPin);
                    }
                    selectedPin = nullptr;
                    state = circuitState::Idle;
                } else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    selectedPin = nullptr;
                    state = circuitState::Idle;
                }
                break;

            default:
                break;
        }
        return;
    }

    if (event.is<sf::Event::MouseButtonReleased>()) {
        switch (state) {
            case circuitState::DraggingComponent:
                if (draggedComponent) {
                    sf::Vector2f newPos = worldPos - dragOffset;
                    draggedComponent->setPosition(snapPosition(newPos));
                    draggedComponent = nullptr;
                    state = circuitState::Idle;
                }
                break;
            default:
                break;
        }
        return;
    }
}

void Circuit::GotoComponent(int id, sf::RenderWindow &window) {
    for (auto &comp: components) {
        if (comp->GetId() == id) {
            sf::Vector2f compPos = comp->getPosition();
            sf::Mouse::setPosition(sf::Vector2i(static_cast<int>(compPos.x), static_cast<int>(compPos.y)), window);
            hoveredComponent = comp.get();
            break;
        }
    }
}

circuitState Circuit::getState() const {
    return state;
}

const std::vector<std::unique_ptr<Component> > &Circuit::GetComponents() const {
    return components;
}

void Circuit::removeComponent(int id) {
    for (size_t i = 0; i < components.size(); i++) {
        if (components[i]->GetId() == id) {
            Component *compToRemove = components[i].get();

            // Remove wires connected to this component's pins
            std::erase_if(wires,
                          [compToRemove](const std::unique_ptr<Wire> &wire) {
                              return wire->getStartPin()->getParent() == compToRemove ||
                                     wire->getEndPin()->getParent() == compToRemove;
                          });

            if (draggedComponent == compToRemove) {
                draggedComponent = nullptr;
            }
            if (selectedPin && selectedPin->getParent() == compToRemove) {
                selectedPin = nullptr;
            }
            if (hoveredComponent == compToRemove) {
                hoveredComponent = nullptr;
            }

            components.erase(components.begin() + i);
            break;
        }
    }
}

int Circuit::getNextId() {
    return nextComponentId++;
}

void Circuit::setNextId(int id) {
    nextComponentId = id;
}

const std::vector<std::unique_ptr<Wire> > &Circuit::getWires() const {
    return wires;
}

Component *Circuit::getComponentById(int id) const {
    for (auto &comp: components) {
        if (comp->GetId() == id) {
            return comp.get();
        }
    }
    return nullptr;
}
