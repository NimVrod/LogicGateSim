#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "../../Component.h"

// SRFlipFlop - Set-Reset Flip Flop
// Inputs: S (Set), R (Reset), CLK (Clock)
// Outputs: Q, Q' (inverted)
class SRFlipFlop : public Component {
public:
    SRFlipFlop(int id, sf::Vector2f position);
    
    void calculate() override;
    std::string GetLabel() override;
    std::string getType() const override;
    ComponentType GetType() override { return ComponentType::SR_FF; }
    void draw(sf::RenderTarget& target) override;
    void drawLabel(sf::RenderTarget& target) override;
    sf::FloatRect getBounds() const override;

protected:
    sf::RectangleShape body;
    
private:
    bool q;           // Current state
    bool lastClk;     // Previous clock state for edge detection
};
