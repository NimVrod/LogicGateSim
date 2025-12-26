#include "../Include/Button.h"
#include <format>

Button::Button(sf::Vector2f position, int output)
	: Component(position)
{
	// Button has one output pin on the right side
	body.setFillColor(sf::Color(255, 0,0));
	body.setSize(sf::Vector2f(40.f, 40.f));
	this->output = output;
	state = false;
	addOutput(sf::Vector2f(40.f, 20.f)); // Assuming button size 40x40
}

void Button::draw(sf::RenderWindow& window, bool shouldDrawPins) {
	if (state)
		body.setFillColor(sf::Color(0, 255, 0));
	else
		body.setFillColor(sf::Color(255, 0, 0));
	body.setPosition(getPosition());
	window.draw(body);
	Component::draw(window, shouldDrawPins);
}

void Button::calculate() {
	for (size_t i = 0; i < outputs.size(); i++)
	{
		if (state)
			outputs[i].get()->setValue(output);
		else
			outputs[i].get()->setValue(0);
	}
}

void Button::toggle() {
	state = !state;
}

std::string Button::GetLabel() {
	return std::format("{} : Button", GetId());
}

sf::FloatRect Button::getBounds() const {
	return sf::FloatRect(m_position, body.getSize());
}