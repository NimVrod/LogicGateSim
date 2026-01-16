#include "../Include/NotGate.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

NotGate::NotGate(int id, sf::Vector2f position)
    : Gate(id, position, 1)
{
    body.setFillColor(sf::Color::Transparent);
    body.setSize(sf::Vector2f(60.f, 40.f));
    
    if (!inputs.empty()) {
        inputs[0]->setRelativePosition(sf::Vector2f(0.f, 20.f));
    }
    if (!outputs.empty()) {
        outputs[0]->setRelativePosition(sf::Vector2f(40.f, 20.f));
    }
}

void NotGate::calculate() {
    int val = inputs[0]->getValue();
    outputs[0]->setValue(!val);
}

std::string NotGate::GetLabel() {
    return std::format("{} : NOT Gate", GetId());
}

std::string NotGate::getType() const {
    return "NotGate";
}

void NotGate::draw(sf::RenderTarget& target) {
    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    
    float h = 40.f;
    float w = 40.f;
    sf::Vector2f pos = getPosition();
    
    triangle.setPoint(0, sf::Vector2f(0, 0));
    triangle.setPoint(1, sf::Vector2f(0, h));
    triangle.setPoint(2, sf::Vector2f(w, h / 2.f));
    
    triangle.setPosition(pos);
    triangle.setFillColor(sf::Color::Transparent); 
    triangle.setOutlineColor(sf::Color::White);
    triangle.setOutlineThickness(2.f);

    float bubbleRadius = 5.f;
    sf::CircleShape bubble(bubbleRadius);
    bubble.setPosition(sf::Vector2f(pos.x + w, pos.y + h/2.f - bubbleRadius));
    bubble.setFillColor(sf::Color::Transparent);
    bubble.setOutlineColor(sf::Color::White);
    bubble.setOutlineThickness(2.f);

    target.draw(triangle);
    target.draw(bubble);
}
