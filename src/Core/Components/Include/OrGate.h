#pragma once

#include "Gate.h"

class OrGate : public Gate {
public:
	OrGate(sf::Vector2f position, int numInputs = 2);
	void calculate() override;
	std::string GetLabel() override;
	void draw(sf::RenderWindow& window) override;
};