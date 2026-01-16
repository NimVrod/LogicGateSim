#pragma once
#include "Component.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

class FlipFlop : public Component {
public:
    FlipFlop(int id, sf::Vector2f position, sf::Vector2f bodySize);
    virtual ~FlipFlop() = default;
    
    void draw(sf::RenderTarget& target) override;
    void drawLabel(sf::RenderTarget& target) override;
    sf::FloatRect getBounds() const override;

protected:
    sf::RectangleShape body;
    bool q = false;
    bool lastClk = false;
    
    virtual std::string getTitle() const = 0;
    virtual void drawPinLabels(sf::RenderTarget& target, sf::Vector2f pos) = 0;
};
