#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "../../Component.h"

// LEDComponent is a visual output that lights up when its input is high.
class LEDComponent : public Component {
public:
    LEDComponent(int id, sf::Vector2f position, sf::Color onColor = sf::Color::Red);
    
    void calculate() override;
    std::string GetLabel() override;
    std::string getType() const override;
    ComponentType GetType() override { return ComponentType::LED; }
    void draw(sf::RenderTarget& target) override;
    void drawLabel(sf::RenderTarget& target) override;
    sf::FloatRect getBounds() const override;
    
    void setOnColor(sf::Color color) { onColor = color; }
    sf::Color getOnColor() const { return onColor; }

protected:
    sf::CircleShape body;
    
private:
    bool lit;
    sf::Color onColor;
    sf::Color offColor;
};
