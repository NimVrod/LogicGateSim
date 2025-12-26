#pragma once

#include "Gate.h"

class NotGate : public Gate {
public:
    NotGate(sf::Vector2f position);
    void calculate() override;
    void draw(sf::RenderWindow& window, bool shouldDrawPins = false) override;
    std::string GetLabel() override;
};
