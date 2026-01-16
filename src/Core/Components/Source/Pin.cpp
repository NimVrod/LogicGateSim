#include "../Include/Pin.h"
#include "../Include/Component.h"
#include <SFML/Graphics/CircleShape.hpp>

const float Pin::RADIUS = 6.f;

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

void Pin::draw(sf::RenderTarget& target) const {
    const sf::Color fillColor = (value >= 1) ? sf::Color::Green : sf::Color(50, 50, 50);
    
    sf::CircleShape shape(RADIUS);
    shape.setOrigin({RADIUS, RADIUS});
    shape.setPosition(getPosition());
    shape.setFillColor(fillColor);
    shape.setOutlineThickness(1.f);
    shape.setOutlineColor(sf::Color::White);

    target.draw(shape);
}
