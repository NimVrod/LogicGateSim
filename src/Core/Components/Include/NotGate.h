#pragma once

#include "Gate.h"

class NotGate : public Gate {
public:
    NotGate(int id, sf::Vector2f position);
    void calculate() override;
    void draw(sf::RenderWindow& window) override;
    std::string GetLabel() override;
};
