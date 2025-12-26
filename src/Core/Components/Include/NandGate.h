#pragma once

#include "Gate.h"

class NandGate : public Gate {
public:
    NandGate(sf::Vector2f position, int numInputs = 2);
    void calculate() override;
    void draw(sf::RenderWindow& window) override;
    std::string GetLabel() override;
};
