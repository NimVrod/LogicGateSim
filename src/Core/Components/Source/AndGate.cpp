#include "../include/AndGate.h"

AndGate::AndGate(sf::Vector2f position, int numInputs)
	: Gate(position, numInputs)
{
	// Set body size based on number of inputs
	body.setFillColor(sf::Color(150, 150, 250));
}
void AndGate::calculate() {
	bool result = true;
	for (const auto& input : inputs) {
		if (input->getValue() == 0) {
			result = false;
			break;
		}
	}
	for (const auto& output : outputs) {
		output->setValue(result ? 1 : 0);
	}
}
std::string AndGate::GetLabel() {
	return std::format("{} : AND Gate", GetId());
}
void AndGate::draw(sf::RenderWindow& window) {
	body.setPosition(getPosition());
	window.draw(body);
	Component::draw(window);
}