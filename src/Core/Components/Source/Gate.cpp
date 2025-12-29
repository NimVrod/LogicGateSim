#include "../Include/Gate.h"

Gate::Gate(int id, sf::Vector2f position, int numInputs)
	: Component(id, position)
{
	if (numInputs < 1) {
		numInputs = 1; // Ensure at least one input
	}

	if (numInputs > 8) {
		numInputs = 8; // Limit to a maximum of 8 
	}

	// Add input pins on the left side
	for (int i = 0; i < numInputs; i++) {
		addInput(sf::Vector2f(0.f, 10.f + i * 20.f));
	}
	// Add one output pin on the right side
	addOutput(sf::Vector2f(60.f, 10.f + (numInputs - 1) * 10.f)); // Centered vertically

	//TODO:
	//Find a better way to set default body properties probably just run Gate::Gate in derived class constructors
	// If not already set, set body size
	if (body.getSize().y == 0 || body.getSize().x == 0) {
		body.setSize(sf::Vector2f(60.f, numInputs * 20.f));
	}

	if (body.getFillColor() == sf::Color::Transparent) {
		body.setFillColor(sf::Color(200, 200, 200));
	}
}

std::string Gate::GetLabel() {
	return std::format("{} : Gate", GetId());
}

sf::FloatRect Gate::getBounds() const {
	return {position, body.getSize()};
}