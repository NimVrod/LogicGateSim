#include "Pin.h"
#include "Component.h"
#include <SFML/Graphics/CircleShape.hpp>

Pin::Pin(Component* parent, PinType type, sf::Vector2f relativePosition)
    : parent(parent), type(type), relativePosition(relativePosition), value(0)
{
}

void Pin::setValue(int value) {
	this->value = value;
}

int Pin::getValue() const {
    return value;
}

PinType Pin::getType() const {
    return type;
}

sf::Vector2f Pin::getPosition() const {
    return parent->getPosition() + relativePosition;
}

void Pin::setRelativePosition(sf::Vector2f relativePosition) {
    this->relativePosition = relativePosition;
}

Component* Pin::getParent() const {
    return parent;
}

void Pin::draw(sf::RenderWindow& window) {
    // Determine color based on state
    sf::Color fillColor = (value >= 1) ? sf::Color::Green : sf::Color(50, 50, 50); // Green for high, dark gray for low
    
    // Maybe different colors for Input vs Output?
    // Inputs are usually where wires end, Outputs where wires start.
    
    sf::CircleShape shape(6.f);
    shape.setOrigin({6.f, 6.f});
    shape.setPosition(getPosition());
    shape.setFillColor(fillColor);
    shape.setOutlineThickness(1.f);
    shape.setOutlineColor(sf::Color::White);

    window.draw(shape);
}
