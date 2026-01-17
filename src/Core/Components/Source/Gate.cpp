#include "../Include/Gate.h"

Gate::Gate(int id, sf::Vector2f position, int numInputs)
    : Component(id, position) {
    if (numInputs < 1) {
        numInputs = 1;
    }

    if (numInputs > 8) {
        numInputs = 8;
    }

    for (int i = 0; i < numInputs; i++) {
        addInput(sf::Vector2f(0.f, 10.f + i * 20.f));
    }

    addOutput(sf::Vector2f(60.f, 10.f + (numInputs - 1) * 10.f));

    if (body.getSize().y == 0 || body.getSize().x == 0) {
        body.setSize(sf::Vector2f(60.f, numInputs * 20.f));
    }

    if (body.getFillColor() == sf::Color::Transparent) {
        body.setFillColor(sf::Color(200, 200, 200));
    }
}

std::string Gate::GetLabel() {
    return std::format("{} : Gate", GetId());
}

sf::FloatRect Gate::getBounds() const {
    return {position, body.getSize()};
}