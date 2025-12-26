#include "Circuit.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <ranges>
#include <Core/Components/Include/Button.h>

void Circuit::addComponent(std::unique_ptr<Component> component) {
    m_components.push_back(std::move(component));
    m_draggedComponent = m_components.back().get();
    m_dragOffset = sf::Vector2f(0.f, 0.f);
    //TODO: Add snap to mouse world pos on start
    state_ = circuitState::DraggingComponent;
}

void Circuit::addWire(Pin* start, Pin* end) {
    if (start && end && start != end) {
        // Basic check: Output -> Input
        if (start->getType() == PinType::Input && end->getType() == PinType::Output) {
            std::swap(start, end);
        }
        
        if (start->getType() == PinType::Output && end->getType() == PinType::Input) {
             m_wires.push_back(std::make_unique<Wire>(start, end));
        }
    }
}

void Circuit::clear() {
    m_wires.clear();
    m_components.clear();
    m_selectedPin = nullptr;
    m_draggedComponent = nullptr;
    state_ = circuitState::Idle;
}

void Circuit::update() {
    for (auto& wire : m_wires) {
        wire->update();
    }
    for (auto& comp : m_components) {
        comp->calculate();
    }
}

void Circuit::draw(sf::RenderWindow& window) {
    // Draw components first, then wires? Wires usually under components or over?
    // Wires usually under.
    for (auto& wire : m_wires) {
        wire->draw(window);
    }
    for (auto& comp : m_components) {
        comp->draw(window);
    }

    if (m_selectedPin) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
        
        sf::Vertex line[] = {
            sf::Vertex{m_selectedPin->getPosition(), sf::Color::Yellow},
            sf::Vertex{worldPos, sf::Color::Yellow}
        };
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }
}

Pin* Circuit::getPinAt(sf::Vector2f pos) {
    const float radius = 10.f; // Tolerance
    for (auto& comp : m_components) {
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
void Circuit::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    switch (state_) {
        case circuitState::Idle:
            if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                    // Check for pin click first (for wire creation)
                    //TODO: Somehow check if a component is over the pin to prevent accidental wire creation when dragging
                    if (Pin* clickedPin = getPinAt(worldPos)) {
                        m_selectedPin = clickedPin;
                        state_ = circuitState::CreatingWire;
                        return;
                    }

                    // Check for component click (for dragging) (start from end for topmost)
                    for (auto & m_component : std::ranges::reverse_view(m_components)) {
                        Component* comp = m_component.get();
                        if (comp->getBounds().contains(worldPos)) {
                            //TODO: Maybe wrap in a function? Add a isClickable() method to Component?
                            //Check if the component is clickable (some components might not be)
                            if (auto* button = dynamic_cast<Button*>(comp)) {
                                // If it's a button, toggle its state only if clicking in center
                                // Make sure that we are in the middle of button to allow dragging from edges
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
                            m_draggedComponent = comp;
                            m_dragOffset = worldPos - comp->getPosition();
                            state_ = circuitState::DraggingComponent;
                            return;
                        }
                    }
                }
                else {
                    //TODO: Do something on right click?
                    //Maybe delete the component?
                }
            }
            else {
                if (event.is<sf::Event::MouseMoved>()) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                    for (auto & m_component : std::ranges::reverse_view(m_components)) {
                        Component* comp = m_component.get();
                        if (comp->getBounds().contains(worldPos)) {
                            //Hovering over component
                        }
                    }
                }
            }
            break;
        case circuitState::DraggingComponent:
            if (event.is<sf::Event::MouseButtonReleased>() && m_draggedComponent) {
                m_draggedComponent->setPosition(
                    sf::Vector2f(
                        sf::Mouse::getPosition(window).x - m_dragOffset.x,
                        sf::Mouse::getPosition(window).y - m_dragOffset.y
                    )
                );
                m_draggedComponent = nullptr;
                state_ = circuitState::Idle;
            }
            else {
                if (m_draggedComponent) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
                    m_draggedComponent->setPosition(worldPos - m_dragOffset);
                    //TODO: Maybe make it a more transparent?
                }
            }
            break;
        case circuitState::CreatingWire:
            if (const auto* mouseButtonPressed = event.getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

                    if (Pin* targetPin = getPinAt(worldPos); targetPin && targetPin != m_selectedPin) {
                        addWire(m_selectedPin, targetPin);
                    }
                    //TODO: Custom nodes for wires
                    m_selectedPin = nullptr;
                    state_ = circuitState::Idle;
                }
                else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    // Cancel wire creation
                    m_selectedPin = nullptr;
                    state_ = circuitState::Idle;
                }
            }
            else if (event.is<sf::Event::MouseButtonReleased>()) {
                //Draw line to mouse pos
                auto start = m_selectedPin->getPosition();
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
                sf::Vertex line[] = {
                    sf::Vertex{start, sf::Color::Yellow},
                    sf::Vertex{worldPos, sf::Color::Yellow}
                };

            }
            break;
    }
}

circuitState Circuit::getState() const {
    return state_;
}

const std::vector<std::unique_ptr<Component>>& Circuit::GetComponents() const {
    return m_components;
}

void Circuit::removeComponent(int id) {
    for (size_t i = 0; i < m_components.size(); i++)
    {
        if (m_components[i]->GetId() == id)
        {
            Component* compToRemove = m_components[i].get();

            // Remove wires connected to this component's pins
            std::erase_if(m_wires,
                          [compToRemove](const std::unique_ptr<Wire>& wire) {
                              return wire->getStartPin()->getParent() == compToRemove ||
                                     wire->getEndPin()->getParent() == compToRemove;
                          });

            // Clear references if the component being removed is selected/dragged
            if (m_draggedComponent == compToRemove) {
                m_draggedComponent = nullptr;
            }
            if (m_selectedPin && m_selectedPin->getParent() == compToRemove) {
                m_selectedPin = nullptr;
            }

            // Swap-and-pop removal
            if (i != m_components.size() - 1) {
                m_components[i] = std::move(m_components.back());
            }
            m_components.pop_back();
            break;
		}
    }
}
