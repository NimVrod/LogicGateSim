#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class Component;

enum class PinType {
    Input,
    Output
};

class Pin {
public:
    static const float RADIUS;

    Pin(Component *parent, PinType type, sf::Vector2f relativePosition);

    void setValue(int value);

    int getValue() const;

    PinType getType() const;

    sf::Vector2f getPosition() const;

    void setRelativePosition(sf::Vector2f relativePosition);

    Component *getParent() const;

    void draw(sf::RenderTarget &target) const;

private:
    Component *parent;
    PinType type;
    sf::Vector2f relativePosition;
    int value;
};