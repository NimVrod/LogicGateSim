#include "Wire.h"
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

Wire::Wire(Pin* start, Pin* end)
    : m_start(start), m_end(end)
{
}

void Wire::update() {
    if (m_start && m_end) {
        m_end->setValue(m_start->getValue());
    }
}

void Wire::draw(sf::RenderWindow& window) {
    if (!m_start || !m_end) return;

    sf::Vector2f startPos = m_start->getPosition();
    sf::Vector2f endPos = m_end->getPosition();

    sf::Color color = (m_start->getValue() >= 1) ? sf::Color::Green : sf::Color(150, 150, 150);

    sf::Vertex line[] = {
        sf::Vertex{startPos, color},
        sf::Vertex{endPos, color}
    };

    window.draw(line, 2, sf::PrimitiveType::Lines);
}
