#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "../../Component.h"

// OutputComponent represents an output pin interface for custom components.
// When used in a sub-circuit, it provides values to external outputs.
class OutputComponent : public Component {
public:
    OutputComponent(int id, sf::Vector2f position, int index = 0);
    
    void calculate() override;
    std::string GetLabel() override;
    std::string getType() const override;
    ComponentType GetType() override { return ComponentType::OUTPUT; }
    void draw(sf::RenderTarget& target) override;
    void drawLabel(sf::RenderTarget& target) override;
    sf::FloatRect getBounds() const override;
    
    int getIndex() const { return index; }
    void setIndex(int idx) { index = idx; }
    
    // Get the computed value (used by CustomComponent to read output)
    int getOutputValue() const { return outputValue; }

protected:
    sf::RectangleShape body;
    
private:
    int index;       // Index for ordering (Output 0, Output 1, etc.)
    int outputValue; // Stored output value from the circuit
};
