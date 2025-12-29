#include "../Include/Button.h"
#include <format>

#include "Core/ResourceManager.h"

Button::Button(int id, sf::Vector2f position, int output)
	: Component(id, position)
{
	// Button has one output pin on the right side
	body.setFillColor(sf::Color(255, 0,0));
	body.setSize(sf::Vector2f(40.f, 40.f));
	this->output = output;
	state = false;
	addOutput(sf::Vector2f(40.f, 20.f)); // Assuming button size 40x40
}

void Button::draw(sf::RenderWindow& window) {
	if (state)
		body.setFillColor(sf::Color(0, 255, 0));
	else
		body.setFillColor(sf::Color(255, 0, 0));
	body.setPosition(getPosition());
	window.draw(body);
}

void Button::drawLabel(sf::RenderWindow &window) {
	ResourceManager& rm = ResourceManager::getInstance();
	sf::Font& font = rm.getFont("assets/ARIAL.TTF");
	sf::Text text(font);
	text.setString(GetLabel());
	text.setCharacterSize(14);
	text.setFillColor(sf::Color::White);
	text.setPosition(sf::Vector2f(position.x, position.y + body.getSize().y + 5.f));
	window.draw(text);
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
	return sf::FloatRect(position, body.getSize());
}