#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "../../Component.h"

// DFlipFlop - Data Flip Flop
// Inputs: D (Data), CLK (Clock)
// Outputs: Q, Q' (inverted)
class DFlipFlop : public Component {
public:
    DFlipFlop(int id, sf::Vector2f position);
    
    void calculate() override;
    std::string GetLabel() override;
    std::string getType() const override;
    ComponentType GetType() override { return ComponentType::D_FF; }
    void draw(sf::RenderTarget& target) override;
    void drawLabel(sf::RenderTarget& target) override;
    sf::FloatRect getBounds() const override;

protected:
    sf::RectangleShape body;
    
private:
    bool q;
    bool lastClk;
};
