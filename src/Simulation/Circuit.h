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
    friend class CircuitSerializer;
public:
    void addComponent(std::unique_ptr<Component> component);
    void addWire(Pin* start, Pin* end);
    void clear();
    const std::vector<std::unique_ptr<Component>>& GetComponents() const;
    const std::vector<std::unique_ptr<Wire>>& getWires() const;
    void removeComponent(int id);

    int getNextId(); // Get the next component ID and increment counter
    void setNextId(int id); // Set the next component ID (for loading)

    void update();
    void draw(sf::RenderWindow& window);
    void setDrawAllPins(bool draw);
    void setDrawLabels(bool draw);
    void handleEvent(const sf::Event &event, sf::RenderWindow &window);
    void GotoComponent(int id, sf::RenderWindow& window);
    circuitState getState() const;
    
    // Grid settings
    void setShowGrid(bool show) { showGrid = show; }
    bool getShowGrid() const { return showGrid; }
    void setSnapToGrid(bool snap) { snapToGrid = snap; }
    bool getSnapToGrid() const { return snapToGrid; }
    void setGridSize(float size) { gridSize = size; }
    float getGridSize() const { return gridSize; }
    
    // Context menu
    int getContextMenuComponentId() const { return contextMenuComponentId; }
    void clearContextMenu() { contextMenuComponentId = -1; }

private:
    std::vector<std::unique_ptr<Component>> components;
    std::vector<std::unique_ptr<Wire>> wires;

    int nextComponentId = 1;

    Pin* selectedPin = nullptr; // For creating wires
    Component* draggedComponent = nullptr;
    circuitState state = circuitState::Idle;
    sf::Vector2f dragOffset;

    Component* hoveredComponent = nullptr;
    bool drawAllPins = false;
    bool drawLabels = false;

    Pin* getPinAt(sf::Vector2f pos);
    void drawGrid(sf::RenderWindow& window);
    sf::Vector2f snapPosition(sf::Vector2f pos) const;
    
    // Grid
    bool showGrid = true;
    bool snapToGrid = true;
    float gridSize = 20.0f;
    
    // Context menu
    int contextMenuComponentId = -1;
};
