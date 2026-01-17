#pragma once
#include "FlipFlop.h"

class DFlipFlop : public FlipFlop {
public:
    DFlipFlop(int id, sf::Vector2f position);

    void calculate() override;

    std::string GetLabel() override;

    std::string getType() const override;

protected:
    std::string getTitle() const override { return "D"; }

    void drawPinLabels(sf::RenderTarget &target, sf::Vector2f pos) override;
};