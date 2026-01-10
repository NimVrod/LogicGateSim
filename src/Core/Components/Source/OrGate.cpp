#include "../Include/OrGate.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include <cmath>

OrGate::OrGate(int id, sf::Vector2f position, int numInputs)
    : Gate(id, position, numInputs)
{
    body.setFillColor(sf::Color::Transparent);
}

void OrGate::calculate() {
    bool result = false;
    for (const auto& input : inputs) {
        if (input->getValue() == 1) {
            result = true;
            break;
        }
    }
    for (const auto& output : outputs) {
        output->setValue(result ? 1 : 0);
    }
}

std::string OrGate::GetLabel() {
    return std::format("{} : OR Gate", GetId());
}

std::string OrGate::getType() const {
    return "OrGate";
}

void OrGate::draw(sf::RenderTarget& target) {
    sf::ConvexShape shape;
    float w = 60.f;
    float h = body.getSize().y;
    sf::Vector2f pos = getPosition();

    std::vector<sf::Vector2f> points;
    
    // Top Curve: (0,0) to (w, h/2)
    // Control point roughly (w/2, 0)
    // Quadratic Bezier: B(t) = (1-t)^2 P0 + 2(1-t)t P1 + t^2 P2
    // P0=(0,0), P1=(30, 0), P2=(60, h/2)
    
    int segments = 20;
    
    // Top Edge
    for (int i = 0; i <= segments; ++i) {
        float t = (float)i / segments;
        float x = (1-t)*(1-t)*0 + 2*(1-t)*t*(w*0.6f) + t*t*w;
        float y = (1-t)*(1-t)*0 + 2*(1-t)*t*0 + t*t*(h/2.f);
        points.push_back(sf::Vector2f(x, y));
    }
    
    // Bottom Edge: (w, h/2) to (0, h)
    // P0=(60, h/2), P1=(30, h), P2=(0, h)
    for (int i = 1; i <= segments; ++i) {
        float t = (float)i / segments;
        float x = (1-t)*(1-t)*w + 2*(1-t)*t*(w*0.6f) + t*t*0;
        float y = (1-t)*(1-t)*(h/2.f) + 2*(1-t)*t*h + t*t*h;
        points.push_back(sf::Vector2f(x, y));
    }
    
    // Back Curve: (0, h) to (0, 0) (Concave)
    // P0=(0, h), P1=(15, h/2), P2=(0, 0)
    for (int i = 1; i < segments; ++i) { // Skip last point (0,0) as it loops to first
        float t = (float)i / segments;
        float x = (1-t)*(1-t)*0 + 2*(1-t)*t*15.f + t*t*0;
        float y = (1-t)*(1-t)*h + 2*(1-t)*t*(h/2.f) + t*t*0;
        points.push_back(sf::Vector2f(x, y));
    }
    
    shape.setPointCount(points.size());
    for (size_t i = 0; i < points.size(); ++i) {
        shape.setPoint(i, points[i]);
    }
    
    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Transparent); 
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.f);
    
    target.draw(shape);
}