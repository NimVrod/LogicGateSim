#include "../Include/XnorGate.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <cmath>

XnorGate::XnorGate(int id, sf::Vector2f position, int numInputs)
    : Gate(id, position, 2)
{
    body.setFillColor(sf::Color::Transparent);
}

void XnorGate::calculate() {
    // XNOR = !XOR
    int ones = 0;
    for (const auto& input : inputs) {
        if (input->getValue() == 1) {
            ones++;
        }
    }
    bool result = (ones % 2 == 1);
    
    for (const auto& output : outputs) {
        output->setValue(result ? 0 : 1);
    }
}

std::string XnorGate::GetLabel() {
    return std::format("{} : XNOR Gate", GetId());
}

void XnorGate::draw(sf::RenderWindow& window) {
    sf::ConvexShape shape;
    
    float shift = 8.f; 
    float w = 55.f - shift; // Shorter for bubble
    float h = body.getSize().y;
    sf::Vector2f pos = getPosition();
    float bubbleRadius = 5.f;

    sf::Vector2f shapePos = pos + sf::Vector2f(shift, 0.f);
    
    std::vector<sf::Vector2f> points;
    int segments = 20;

    // Top Edge
    for (int i = 0; i <= segments; ++i) {
        float t = (float)i / segments;
        float x = (1-t)*(1-t)*0 + 2*(1-t)*t*(w*0.6f) + t*t*w;
        float y = (1-t)*(1-t)*0 + 2*(1-t)*t*0 + t*t*(h/2.f);
        points.push_back(sf::Vector2f(x, y));
    }
    // Bottom Edge
    for (int i = 1; i <= segments; ++i) {
        float t = (float)i / segments;
        float x = (1-t)*(1-t)*w + 2*(1-t)*t*(w*0.6f) + t*t*0;
        float y = (1-t)*(1-t)*(h/2.f) + 2*(1-t)*t*h + t*t*h;
        points.push_back(sf::Vector2f(x, y));
    }
    // Back Curve
    for (int i = 1; i < segments; ++i) {
        float t = (float)i / segments;
        float x = (1-t)*(1-t)*0 + 2*(1-t)*t*15.f + t*t*0;
        float y = (1-t)*(1-t)*h + 2*(1-t)*t*(h/2.f) + t*t*0;
        points.push_back(sf::Vector2f(x, y));
    }
    
    shape.setPointCount(points.size());
    for (size_t i = 0; i < points.size(); ++i) {
        shape.setPoint(i, points[i]);
    }
    
    shape.setPosition(shapePos);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.f);
    
    window.draw(shape);
    
    // Extra Curve
    sf::VertexArray extraCurve(sf::PrimitiveType::LineStrip, segments + 1);
    for (int i = 0; i <= segments; ++i) {
         float t = (float)i / segments;
         float x = (1-t)*(1-t)*0 + 2*(1-t)*t*15.f + t*t*0;
         float y = (1-t)*(1-t)*h + 2*(1-t)*t*(h/2.f) + t*t*0; 
         extraCurve[i].position = pos + sf::Vector2f(x, y);
         extraCurve[i].color = sf::Color::White;
    }
    window.draw(extraCurve);
    
    // Bubble
    sf::CircleShape bubble(bubbleRadius);
    bubble.setPosition(sf::Vector2f(shapePos.x + w, shapePos.y + h/2.f - bubbleRadius));
    bubble.setFillColor(sf::Color::Transparent);
    bubble.setOutlineColor(sf::Color::White);
    bubble.setOutlineThickness(2.f);
    window.draw(bubble);
}
