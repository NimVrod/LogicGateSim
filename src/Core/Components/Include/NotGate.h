#pragma once

#include "Gate.h"

class NotGate : public Gate {
public:
    NotGate(int id, sf::Vector2f position);

    void calculate() override;

    void draw(sf::RenderTarget &target) override;

    std::string GetLabel() override;

    std::string getType() const override;
};