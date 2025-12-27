#include "Circuit.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <ranges>
#include <Core/Components/Include/Button.h>

void Circuit::addComponent(std::unique_ptr<Component> component) {
    components.push_back(std::move(component));
    draggedComponent = components.back().get();
    dragOffset = sf::Vector2f(0.f, 0.f);
    //TODO: Add snap to mouse world pos on start
    state = circuitState::DraggingComponent;
}

void Circuit::addWire(Pin* start, Pin* end) {
    if (start && end && start != end) {
        // Basic check: Output -> Input
        if (start->getType() == PinType::Input && end->getType() == PinType::Output) {
            std::swap(start, end);
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
}

void Circuit::update() {
    for (auto& wire : wires) {
        wire->update();
    }
    for (auto& comp : components) {
        comp->calculate();
    }
}

void Circuit::draw(sf::RenderWindow& window) {
    // Draw components first, then wires? Wires usually under components or over?
    // Wires usually under.
    for (auto& wire : wires) {
        wire->draw(window);
    }
    for (auto& comp : components) {
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
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
        
        sf::Vertex line[] = {
            sf::Vertex{selectedPin->getPosition(), sf::Color::Yellow},
            sf::Vertex{worldPos, sf::Color::Yellow}
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }
}

void Circuit::setDrawAllPins(bool draw) {
    drawAllPins = draw;
}

void Circuit::setDrawLabels(bool draw) {
    drawLabels = draw;
}

Pin* Circuit::getPinAt(sf::Vector2f pos) {
    const float radius = 10.f; // Tolerance
    for (auto& comp : components) {
        for (auto& pin : comp->getInputs()) {
            sf::Vector2f pinPos = pin->getPosition();
            float distSq = (pos.x - pinPos.x)*(pos.x - pinPos.x) + (pos.y - pinPos.y)*(pos.y - pinPos.y);
            if (distSq < radius * radius) return pin.get();
        }
        for (auto& pin : comp->getOutputs()) {
            sf::Vector2f pinPos = pin->getPosition();
            float distSq = (pos.x - pinPos.x)*(pos.x - pinPos.x) + (pos.y - pinPos.y)*(pos.y - pinPos.y);
            if (distSq < radius * radius) return pin.get();
        }
    }
    return nullptr;
}
void Circuit::handleEvent(const sf::Event &event, sf::RenderWindow &window) {
    // Get mouse position once for reuse
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

    // Handle MouseMoved events - hover detection works in all states
    if (event.is<sf::Event::MouseMoved>()) {
        hoveredComponent = nullptr;
        for (auto & component : std::ranges::reverse_view(components)) {
            Component* comp = component.get();
            if (comp->getBounds().contains(worldPos)) {
                hoveredComponent = comp;
                break;
            }
        }

        // State-specific mouse move handling
        switch (state) {
            case circuitState::DraggingComponent:
                if (draggedComponent) {
                    draggedComponent->setPosition(worldPos - dragOffset);
                }
                break;
            default:
                break;
        }
        return;
    }

    // Handle MouseButtonPressed events
    if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        switch (state) {
            case circuitState::Idle:
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    // Check for pin click first (for wire creation)
                    if (Pin* clickedPin = getPinAt(worldPos)) {
                        selectedPin = clickedPin;
                        state = circuitState::CreatingWire;
                        return;
                    }

                    // Check for component click (for dragging) (start from end for topmost)
                    for (auto & component : std::ranges::reverse_view(components)) {
                        Component* comp = component.get();
                        if (comp->getBounds().contains(worldPos)) {
                            // Check if the component is clickable (some components might not be)
                            if (auto* button = dynamic_cast<Button*>(comp)) {
                                // If it's a button, toggle its state only if clicking in center
                                sf::FloatRect bounds = button->getBounds();
                                // Create a smaller inner rect (60% of size, centered)
                                float margin = bounds.size.x * 0.2f;
                                sf::FloatRect innerBounds(
                                    sf::Vector2f(bounds.position.x + margin, bounds.position.y + margin),
                                    sf::Vector2f(bounds.size.x - 2 * margin, bounds.size.y - 2 * margin)
                                );
                                if (innerBounds.contains(worldPos)) {
                                    button->toggle();
                                    return;
                                }
                                // Fall through to dragging if clicking on edges
                            }
                            draggedComponent = comp;
                            dragOffset = worldPos - comp->getPosition();
                            state = circuitState::DraggingComponent;
                            return;
                        }
                    }
                }
                break;

            case circuitState::CreatingWire:
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    if (Pin* targetPin = getPinAt(worldPos); targetPin && targetPin != selectedPin) {
                        addWire(selectedPin, targetPin);
                    }
                    selectedPin = nullptr;
                    state = circuitState::Idle;
                }
                else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    // Cancel wire creation
                    selectedPin = nullptr;
                    state = circuitState::Idle;
                }
                break;

            default:
                break;
        }
        return;
    }

    // Handle MouseButtonReleased events
    if (event.is<sf::Event::MouseButtonReleased>()) {
        switch (state) {
            case circuitState::DraggingComponent:
                if (draggedComponent) {
                    draggedComponent->setPosition(worldPos - dragOffset);
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

circuitState Circuit::getState() const {
    return state;
}

const std::vector<std::unique_ptr<Component>>& Circuit::GetComponents() const {
    return components;
}

void Circuit::removeComponent(int id) {
    for (size_t i = 0; i < components.size(); i++)
    {
        if (components[i]->GetId() == id)
        {
            Component* compToRemove = components[i].get();

            // Remove wires connected to this component's pins
            std::erase_if(wires,
                          [compToRemove](const std::unique_ptr<Wire>& wire) {
                              return wire->getStartPin()->getParent() == compToRemove ||
                                     wire->getEndPin()->getParent() == compToRemove;
                          });

            // Clear references if the component being removed is selected/dragged
            if (draggedComponent == compToRemove) {
                draggedComponent = nullptr;
            }
            if (selectedPin && selectedPin->getParent() == compToRemove) {
                selectedPin = nullptr;
            }
            if (hoveredComponent == compToRemove) {
                hoveredComponent = nullptr;
            }

            // Swap-and-pop removal
            if (i != components.size() - 1) {
                components[i] = std::move(components.back());
            }
            components.pop_back();
            break;
		}
    }
}
