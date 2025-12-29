#pragma once

#include "Gate.h"

class XorGate : public Gate {
public:
    XorGate(int id, sf::Vector2f position, int numInputs = 2); // Typically 2 inputs for basic XOR
    void calculate() override;
    void draw(sf::RenderWindow& window) override;
    std::string GetLabel() override;
};
