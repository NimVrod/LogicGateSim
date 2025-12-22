#pragma once
#include <vector>
#include <memory>
#include "../Core/Component.h"
#include "../Core/Wire.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class Circuit {
public:
    void addComponent(std::unique_ptr<Component> component);
    void addWire(Pin* start, Pin* end);
    void clear();

    void update();
    void draw(sf::RenderWindow& window);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

private:
    std::vector<std::unique_ptr<Component>> m_components;
    std::vector<std::unique_ptr<Wire>> m_wires;

    Pin* m_selectedPin = nullptr; // For creating wires
    Component* m_draggedComponent = nullptr;
    sf::Vector2f m_dragOffset;

    Pin* getPinAt(sf::Vector2f pos);
};
