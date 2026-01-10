#pragma once

#include "Gate.h"

class AndGate : public Gate {
public:
	AndGate(int id, sf::Vector2f position, int numInputs = 2);
	void calculate() override;
	std::string GetLabel() override;
	std::string getType() const override;
	void draw(sf::RenderTarget& target) override;
};