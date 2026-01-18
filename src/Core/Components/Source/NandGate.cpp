#include "../Include/NandGate.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <cmath>

NandGate::NandGate(int id, sf::Vector2f position, int numInputs)
    : Gate(id, position, numInputs) {
    body.setFillColor(sf::Color::Transparent);
}

void NandGate::calculate() {
    bool result = true;
    for (const auto &input: inputs) {
        if (input->getValue() == 0) {
            result = false;
            break;
        }
    }
    // NAND = !AND
    for (const auto &output: outputs) {
        output->setValue(result ? 0 : 1);
    }
}

std::string NandGate::GetLabel() {
    return std::format("{} : NAND Gate", GetId());
}

std::string NandGate::getType() const {
    return "NandGate";
}

void NandGate::draw(sf::RenderTarget &target) {
    sf::ConvexShape shape;
    float h = body.getSize().y;
    float shapeW = 50.f;
    float bubbleRadius = 5.f;
    sf::Vector2f pos = getPosition();

    std::vector<sf::Vector2f> points;
    float curveStart = shapeW * 0.5f;

    points.push_back(sf::Vector2f(0.f, 0.f));

    int segments = 20;
    for (int i = 0; i <= segments; ++i) {
        float angle = -90.f + (180.f * i / segments);
        float rad = angle * 3.14159f / 180.f;
        float rx = shapeW - curveStart;
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
    shape.setOutlineColor(sf::Color::White);
    shape.setOutlineThickness(2.f);

    sf::CircleShape bubble(bubbleRadius);
    bubble.setPosition(sf::Vector2f(pos.x + shapeW, pos.y + h / 2.f - bubbleRadius));
    bubble.setFillColor(sf::Color::Transparent);
    bubble.setOutlineColor(sf::Color::White);
    bubble.setOutlineThickness(2.f);

    target.draw(shape);
    target.draw(bubble);
}