#include "Wire.h"
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

Wire::Wire(Pin* start, Pin* end)
    : start(start), end(end)
{
}

void Wire::update() {
    if (start && end) {
        end->setValue(start->getValue());
    }
}

void Wire::draw(sf::RenderWindow& window) {
    if (!start || !end) return;

    sf::Vector2f startPos = start->getPosition();
    sf::Vector2f endPos = end->getPosition();

    sf::Color color = (start->getValue() >= 1) ? sf::Color::Green : sf::Color(150, 150, 150);

    sf::Vertex line[] = {
        sf::Vertex{startPos, color},
        sf::Vertex{endPos, color}
    };

    window.draw(line, 2, sf::PrimitiveType::Lines);
}
