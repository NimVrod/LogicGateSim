#include "../Include/XorGate.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <cmath>

XorGate::XorGate(int id, sf::Vector2f position, int numInputs)
    : Gate(id, position, 2) {
    body.setFillColor(sf::Color::Transparent);
}

void XorGate::calculate() {
    int ones = 0;
    for (const auto &input: inputs) {
        if (input->getValue() == 1) {
            ones++;
        }
    }
    bool result = (ones % 2 == 1);

    for (const auto &output: outputs) {
        output->setValue(result ? 1 : 0);
    }
}

std::string XorGate::GetLabel() {
    return std::format("{} : XOR Gate", GetId());
}

std::string XorGate::getType() const {
    return "XorGate";
}

void XorGate::draw(sf::RenderTarget &target) {
    sf::ConvexShape shape;

    float shift = 8.f;
    float w = 60.f - shift;
    float h = body.getSize().y;
    sf::Vector2f pos = getPosition();
    sf::Vector2f shapePos = pos + sf::Vector2f(shift, 0.f);

    std::vector<sf::Vector2f> points;
    int segments = 20;

    for (int i = 0; i <= segments; ++i) {
        float t = (float) i / segments;
        float x = (1 - t) * (1 - t) * 0 + 2 * (1 - t) * t * (w * 0.6f) + t * t * w;
        float y = (1 - t) * (1 - t) * 0 + 2 * (1 - t) * t * 0 + t * t * (h / 2.f);
        points.push_back(sf::Vector2f(x, y));
    }

    for (int i = 1; i <= segments; ++i) {
        float t = (float) i / segments;
        float x = (1 - t) * (1 - t) * w + 2 * (1 - t) * t * (w * 0.6f) + t * t * 0;
        float y = (1 - t) * (1 - t) * (h / 2.f) + 2 * (1 - t) * t * h + t * t * h;
        points.push_back(sf::Vector2f(x, y));
    }

    for (int i = 1; i < segments; ++i) {
        float t = (float) i / segments;
        float x = (1 - t) * (1 - t) * 0 + 2 * (1 - t) * t * 15.f + t * t * 0;
        float y = (1 - t) * (1 - t) * h + 2 * (1 - t) * t * (h / 2.f) + t * t * 0;
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

    target.draw(shape);

    sf::VertexArray extraCurve(sf::PrimitiveType::LineStrip, segments + 1);
    for (int i = 0; i <= segments; ++i) {
        float t = (float) i / segments;
        float x = (1 - t) * (1 - t) * 0 + 2 * (1 - t) * t * 15.f + t * t * 0;
        float y = (1 - t) * (1 - t) * h + 2 * (1 - t) * t * (h / 2.f) + t * t * 0;
        extraCurve[i].position = pos + sf::Vector2f(x, y);
        extraCurve[i].color = sf::Color::White;
    }

    target.draw(extraCurve);
}