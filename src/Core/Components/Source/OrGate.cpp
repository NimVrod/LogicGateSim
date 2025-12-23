#include "../Include/OrGate.h"

OrGate::OrGate(sf::Vector2f position, int numInputs)
	: Gate(position, numInputs)
{
	body.setFillColor(sf::Color(250, 150, 150));
}

void OrGate::calculate() {
	bool result = false;
	for (const auto& input : inputs) {
		if (input->getValue() != 0) {
			result = true;
			break;
		}
	}
	for (const auto& output : outputs) {
		output->setValue(result ? 1 : 0);
	}
}

std::string OrGate::GetLabel() {
	return std::format("{} : OR Gate", GetId());
}
void OrGate::draw(sf::RenderWindow& window) {
	body.setPosition(getPosition());
	window.draw(body);
	Component::draw(window);
}