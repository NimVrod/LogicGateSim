#include "../include/AndGate.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include <cmath>

AndGate::AndGate(int id, sf::Vector2f position, int numInputs)
    : Gate(id, position, numInputs) {
    body.setFillColor(sf::Color::Transparent);
}

void AndGate::calculate() {
    bool result = true;
    for (const auto &input: inputs) {
        if (input->getValue() == 0) {
            result = false;
            break;
        }
    }
    for (const auto &output: outputs) {
        output->setValue(result ? 1 : 0);
    }
}

std::string AndGate::GetLabel() {
    return std::format("{} : AND Gate", GetId());
}

std::string AndGate::getType() const {
    return "AndGate";
}

void AndGate::draw(sf::RenderTarget &target) {
    sf::ConvexShape shape;

    float w = 60.f;
    float h = 0.f;
    if (!inputs.empty()) {
        h = inputs.size() * 20.f;
    } else {
        h = 40.f;
    }
    if (h < 40.f) h = 40.f;

    h = body.getSize().y;

    sf::Vector2f pos = getPosition();

    std::vector<sf::Vector2f> points;

    float curveStart = w * 0.5f;
    points.push_back(sf::Vector2f(0.f, 0.f));

    int segments = 20;
    for (int i = 0; i <= segments; ++i) {
        float angle = -90.f + (180.f * i / segments);
        float rad = angle * 3.14159f / 180.f;

        float rx = w - curveStart;
        float ry = h / 2.f;

        float x = curveStart + rx * std::cos(rad);
        float y = h / 2.f + ry * std::sin(rad);

        points.push_back(sf::Vector2f(x, y));
    }

    points.push_back(sf::Vector2f(0.f, h));

    shape.setPointCount(points.size());
    for (size_t i = 0; i < points.size(); ++i) {
        shape.setPoint(i, points[i]);
    }

    shape.setPosition(pos);
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);
    target.draw(shape);
}