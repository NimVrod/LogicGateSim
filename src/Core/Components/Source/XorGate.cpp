#include "../Include/XorGate.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <cmath>

XorGate::XorGate(int id, sf::Vector2f position, int numInputs)
    : Gate(id, position, 2)
{
    body.setFillColor(sf::Color::Transparent);
}

void XorGate::calculate() {
    // XOR: Odd number of 1s returns 1, or just A != B for 2 inputs.
    // For n>2, usually parity (odd number of inputs are 1).
    int ones = 0;
    for (const auto& input : inputs) {
        if (input->getValue() == 1) {
            ones++;
        }
    }
    bool result = (ones % 2 == 1);
    
    for (const auto& output : outputs) {
        output->setValue(result ? 1 : 0);
    }
}

std::string XorGate::GetLabel() {
    return std::format("{} : XOR Gate", GetId());
}

std::string XorGate::getType() const {
    return "XorGate";
}

void XorGate::draw(sf::RenderWindow& window) {
    // OR Shape shifted right + Extra curve line
    sf::ConvexShape shape;
    
    float shift = 8.f; // Gap size
    float w = 60.f - shift; // Adjust width to stay within 60 overall roughly
    float h = body.getSize().y;
    sf::Vector2f pos = getPosition();
    
    // Position the OR shape slightly to the right
    sf::Vector2f shapePos = pos + sf::Vector2f(shift, 0.f);
    
    // Generate OR shape points (same as OrGate)
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
    // Back Curve (Concave)
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
    
    // Draw the extra curve at the back
    // Curve similar to the back curve of OR shape, but at x=0
    sf::VertexArray extraCurve(sf::PrimitiveType::LineStrip, segments + 1);
    
    for (int i = 0; i <= segments; ++i) {
         float t = (float)i / segments;
         float x = (1-t)*(1-t)*0 + 2*(1-t)*t*15.f + t*t*0;
         float y = (1-t)*(1-t)*h + 2*(1-t)*t*(h/2.f) + t*t*0; // y goes h -> 0
         // Wait, the previous back curve loop was i=1 to <segments for closing polygon
         // Direction: y goes h -> 0 in that loop?
         // Let's recheck Bezier P0->P2. P0=(0,h), P2=(0,0). Yes.
         
         // We want this curve.
         extraCurve[i].position = pos + sf::Vector2f(x, y);
         extraCurve[i].color = sf::Color::White;
    }
    
    // SFML LineStrip doesn't support thickness natively.
    // We can draw it multiple times or use a thin quad strip if needed.
    // For now simple line strip.
    window.draw(extraCurve);
}
