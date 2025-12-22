#pragma once
#include "Component.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

class Gate : public Component {
public:
    Gate(sf::Vector2f position, const std::string& label);
    void draw(sf::RenderWindow& window) override;

protected:
    sf::RectangleShape m_body;
    // We might want a label or different shape
    // For simplicity, just a rectangle with text
    // Requires loading font, which is tricky in global scope. 
    // Maybe just color code or simple shape for now.
};

class AndGate : public Gate {
public:
    AndGate(sf::Vector2f position);
    void calculate() override;
};

class OrGate : public Gate {
public:
    OrGate(sf::Vector2f position);
    void calculate() override;
};

class NotGate : public Gate {
public:
    NotGate(sf::Vector2f position);
    void calculate() override;
};
