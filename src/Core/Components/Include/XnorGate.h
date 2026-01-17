#pragma once

#include "Gate.h"

class XnorGate : public Gate {
public:
    XnorGate(int id, sf::Vector2f position, int numInputs = 2);

    void calculate() override;

    void draw(sf::RenderTarget &target) override;

    std::string GetLabel() override;

    std::string getType() const override;
};