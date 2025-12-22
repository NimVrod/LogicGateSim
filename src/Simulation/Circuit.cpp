#include "Circuit.h"
#include "../Core/IOComponents.h"
#include "../Core/Gates.h"
#include <iostream>
#include <cmath>

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
    if (event.is<sf::Event::MouseButtonPressed>()) {
        const auto& mouseEvent = event.getIf<sf::Event::MouseButtonPressed>();
        if (mouseEvent->button == sf::Mouse::Button::Left) {
            sf::Vector2f worldPos = window.mapPixelToCoords(mouseEvent->position);

            // 1. Check Pins (Priority over drag/click)
            Pin* clickedPin = getPinAt(worldPos);
            if (clickedPin) {
                if (m_selectedPin) {
                    addWire(m_selectedPin, clickedPin);
                    m_selectedPin = nullptr;
                } else {
                    m_selectedPin = clickedPin;
                }
                return; 
            }
            
            // Cancel wiring if clicked elsewhere
            m_selectedPin = nullptr;

            // 2. Check Buttons
            for (auto& comp : m_components) {
                if (auto* btn = dynamic_cast<Button*>(comp.get())) {
                    if (btn->isMouseOver(worldPos)) {
                        btn->toggle();
                        return;
                    }
                }
                
                // 3. Dragging
                // Simple box check for general component based on pos
                sf::Vector2f pos = comp->getPosition();
                // Assume generic size of 60x40 for Gates, 40x40 for Button
                // This is a bit hacky, but 'Component' doesn't expose bounds yet. 
                // We'll rely on the fact that pos is top-left.
                if (worldPos.x >= pos.x && worldPos.x <= pos.x + 60.f &&
                    worldPos.y >= pos.y && worldPos.y <= pos.y + 40.f) {
                    m_draggedComponent = comp.get();
                    m_dragOffset = pos - worldPos;
                    return;
                }
            }
        }
    } else if (event.is<sf::Event::MouseButtonReleased>()) {
        m_draggedComponent = nullptr;
    } else if (event.is<sf::Event::MouseMoved>()) {
         if (m_draggedComponent) {
             const auto& moveEvent = event.getIf<sf::Event::MouseMoved>();
             sf::Vector2f worldPos = window.mapPixelToCoords(moveEvent->position);
             m_draggedComponent->setPosition(worldPos + m_dragOffset);
         }
    }
}
