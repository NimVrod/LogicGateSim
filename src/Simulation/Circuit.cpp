#include "Circuit.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <Core/Components/Include/Button.h>

void Circuit::addComponent(std::unique_ptr<Component> component) {
    m_components.push_back(std::move(component));
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
//TODO: Refactor this monstrous function
void Circuit::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.is<sf::Event::MouseButtonPressed>()) {
        const auto& mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2f worldPos = window.mapPixelToCoords(mouseEvent->position);

            // 1. Check Components first (backwards = topmost first)
            for (auto it = m_components.rbegin(); it != m_components.rend(); ++it) {
                auto& comp = *it;
                if (!comp->getBounds().contains(worldPos)) continue;
                
                // Check if clicking on this component's own pin - allow wiring
                Pin* ownPin = nullptr;
                const float pinRadius = 10.f;
                for (auto& pin : comp->getInputs()) {
                    sf::Vector2f pinPos = pin->getPosition();
                    float distSq = (worldPos.x - pinPos.x)*(worldPos.x - pinPos.x) + 
                                   (worldPos.y - pinPos.y)*(worldPos.y - pinPos.y);
                    if (distSq < pinRadius * pinRadius) { ownPin = pin.get(); break; }
                }
                if (!ownPin) {
                    for (auto& pin : comp->getOutputs()) {
                        sf::Vector2f pinPos = pin->getPosition();
                        float distSq = (worldPos.x - pinPos.x)*(worldPos.x - pinPos.x) + 
                                       (worldPos.y - pinPos.y)*(worldPos.y - pinPos.y);
                        if (distSq < pinRadius * pinRadius) { ownPin = pin.get(); break; }
                    }
                }
                
                if (ownPin) {
                    // Wire this pin
                    if (m_selectedPin) {
                        addWire(m_selectedPin, ownPin);
                        m_selectedPin = nullptr;
                    } else {
                        m_selectedPin = ownPin;
                    }
                    return;
                }
                
                // Button: toggle if clicking center, else drag
                if (Button* button = dynamic_cast<Button*>(comp.get())) {
                    sf::Vector2f pos = button->getPosition();
                    sf::Vector2f size = button->getBounds().size;
                    float m = 10.f;
                    
                    bool inCenter = worldPos.x > pos.x + m && worldPos.x < pos.x + size.x - m &&
                                    worldPos.y > pos.y + m && worldPos.y < pos.y + size.y - m;
                    if (inCenter) {
                        button->toggle();
                        m_selectedPin = nullptr;
                        return;
                    }
                }
                
                // Start dragging
                m_draggedComponent = comp.get();
                m_dragOffset = comp->getPosition() - worldPos;
                m_selectedPin = nullptr;
                return;
            }
            
            // 2. Clicked on empty space - cancel wiring
            m_selectedPin = nullptr;
        }
    } else if (event.is<sf::Event::MouseButtonReleased>()) {
        if (m_draggedComponent) {
            // Move dragged component to end so it's drawn on top
            for (auto it = m_components.begin(); it != m_components.end(); ++it) {
                if (it->get() == m_draggedComponent) {
                    auto comp = std::move(*it);
                    m_components.erase(it);
                    m_components.push_back(std::move(comp));
                    break;
                }
            }
            m_draggedComponent = nullptr;
        }
    } else if (event.is<sf::Event::MouseMoved>()) {
         if (m_draggedComponent) {
             const auto& moveEvent = event.getIf<sf::Event::MouseMoved>();
             sf::Vector2f worldPos = window.mapPixelToCoords(moveEvent->position);
             m_draggedComponent->setPosition(worldPos + m_dragOffset);
         }
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
            m_wires.erase(
                std::remove_if(m_wires.begin(), m_wires.end(),
                    [compToRemove](const std::unique_ptr<Wire>& wire) {
                        return wire->getStartPin()->getParent() == compToRemove ||
                               wire->getEndPin()->getParent() == compToRemove;
                    }),
                m_wires.end());

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
