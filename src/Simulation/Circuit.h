#pragma once
#include <vector>
#include <memory>
#include "../Core/Component.h"
#include "../Core/Wire.h"
#include <SFML/Graphics/RenderWindow.hpp>

enum class circuitState {
    Idle,
    DraggingComponent,
    CreatingWire
};
class Circuit {
public:
    void addComponent(std::unique_ptr<Component> component);
    void addWire(Pin* start, Pin* end);
    void clear();
    const std::vector<std::unique_ptr<Component>>& GetComponents() const;
    void removeComponent(int id);

    void update();
    void draw(sf::RenderWindow& window);
    void setDrawAllPins(bool draw);
    void setDrawLabels(bool draw);
    void handleEvent(const sf::Event &event, sf::RenderWindow &window);
    void GotoComponent(int id, sf::RenderWindow& window);
    circuitState getState() const;

private:
    std::vector<std::unique_ptr<Component>> components;
    std::vector<std::unique_ptr<Wire>> wires;

    Pin* selectedPin = nullptr; // For creating wires
    Component* draggedComponent = nullptr;
    circuitState state = circuitState::Idle;
    sf::Vector2f dragOffset;

    Component* hoveredComponent = nullptr;
    bool drawAllPins = false;
    bool drawLabels = false;

    Pin* getPinAt(sf::Vector2f pos);
};
