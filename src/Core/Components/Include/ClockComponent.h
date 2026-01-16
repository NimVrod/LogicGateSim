#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Component.h"

// ClockComponent generates a toggling signal at a configurable rate.
// It acts as a time-based input for sequential circuits.
class ClockComponent : public Component {
public:
    ClockComponent(int id, sf::Vector2f position, float frequency = 1.0f);
    
    void calculate() override;
    std::string GetLabel() override;
    std::string getType() const override;
    void draw(sf::RenderTarget& target) override;
    void drawLabel(sf::RenderTarget& target) override;
    sf::FloatRect getBounds() const override;
    
    // Clock control
    void tick();  // Called by simulation to advance clock
    void setFrequency(float hz) { frequency = hz; }
    float getFrequency() const { return frequency; }
    
    bool getState() const { return state; }

protected:
    sf::RectangleShape body;
    
private:
    bool state;
    float frequency;     // Ticks per simulation update
    float accumulator;   // For sub-tick timing
};
