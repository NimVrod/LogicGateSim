#include "../Include/NotGate.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

NotGate::NotGate(sf::Vector2f position)
    : Gate(position, 1) // NOT gate always has 1 input
{
    body.setFillColor(sf::Color::Transparent); // Hide default rect
    
    // Fix body size to match drawing height (40)
    body.setSize(sf::Vector2f(60.f, 40.f));
    
    // Adjust pin positions to center of 40-height
    if (!inputs.empty()) {
        inputs[0]->setRelativePosition(sf::Vector2f(0.f, 20.f));
    }
    if (!outputs.empty()) {
        outputs[0]->setRelativePosition(sf::Vector2f(60.f, 20.f));
    }
}

void NotGate::calculate() {
    int val = inputs[0]->getValue();
    outputs[0]->setValue(!val);
}

std::string NotGate::GetLabel() {
    return std::format("{} : NOT Gate", GetId());
}

void NotGate::draw(sf::RenderWindow& window) {
    // Draw Triangle
    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    
    float h = 40.f; // Standard height
    float w = 40.f; // Width of triangle
    
    // Adjust based on actual position
    sf::Vector2f pos = getPosition();
    
    triangle.setPoint(0, sf::Vector2f(0, 0));
    triangle.setPoint(1, sf::Vector2f(0, h));
    triangle.setPoint(2, sf::Vector2f(w, h / 2.f));
    
    triangle.setPosition(pos);
    triangle.setFillColor(sf::Color::Transparent); 
    triangle.setOutlineColor(sf::Color::White);
    triangle.setOutlineThickness(2.f);

    // Draw Bubble
    float bubbleRadius = 5.f;
    sf::CircleShape bubble(bubbleRadius);
    bubble.setPosition(sf::Vector2f(pos.x + w, pos.y + h/2.f - bubbleRadius));
    bubble.setFillColor(sf::Color::Transparent);
    bubble.setOutlineColor(sf::Color::White);
    bubble.setOutlineThickness(2.f);

    window.draw(triangle);
    window.draw(bubble);
}
