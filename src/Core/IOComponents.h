#pragma once
#include "Component.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class Button : public Component {
public:
    Button(sf::Vector2f position);
    void calculate() override;
    void draw(sf::RenderWindow& window) override;
    
    void toggle();
    bool isMouseOver(sf::Vector2f mousePos) const;

private:
    bool m_state;
    sf::RectangleShape m_body;
};

class Led : public Component {
public:
    Led(sf::Vector2f position);
    void calculate() override;
    void draw(sf::RenderWindow& window) override;

private:
    sf::CircleShape m_body;
    sf::Color m_offColor;
    sf::Color m_onColor;
};
