#pragma once

#include "Gate.h"

class NorGate : public Gate {
public:
    NorGate(sf::Vector2f position, int numInputs = 2);
    void calculate() override;
    void draw(sf::RenderWindow& window, bool shouldDrawPins = false) override;
    std::string GetLabel() override;
};
