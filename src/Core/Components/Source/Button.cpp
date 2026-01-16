#include "../Include/Button.h"
#include <format>
#include "../Include/ResourceManager.h"

Button::Button(int id, sf::Vector2f position, int output)
	: Component(id, position)
{
	body.setFillColor(sf::Color(180, 60, 60));
	body.setSize(sf::Vector2f(50.f, 40.f));
	body.setOutlineColor(sf::Color::White);
	body.setOutlineThickness(2.f);
	this->output = output;
	state = false;
	addOutput(sf::Vector2f(50.f, 20.f));
}

void Button::draw(sf::RenderTarget& target) {
	sf::Vector2f pos = getPosition();
	
	if (state) {
		body.setFillColor(sf::Color(80, 180, 80));
	} else {
		body.setFillColor(sf::Color(180, 60, 60));
	}
	
	body.setPosition(pos);
	target.draw(body);
	
	ResourceManager& rm = ResourceManager::getInstance();
	sf::Font& font = rm.getDefaultFont();
	sf::Text text(font);
	text.setString(state ? "ON" : "OFF");
	text.setCharacterSize(12);
	text.setFillColor(sf::Color::White);
	text.setPosition(sf::Vector2f(pos.x + 12.f, pos.y + 12.f));
	target.draw(text);
}

void Button::drawLabel(sf::RenderTarget& target) {
	ResourceManager& rm = ResourceManager::getInstance();
	sf::Font& font = rm.getDefaultFont();
	sf::Text text(font);
	text.setString(GetLabel());
	text.setCharacterSize(14);
	text.setFillColor(sf::Color::White);
	text.setPosition(sf::Vector2f(position.x, position.y + body.getSize().y + 5.f));
	target.draw(text);
}

void Button::calculate() {
	for (const auto& out : outputs) {
		out->setValue(state ? output : 0);
	}
}

void Button::toggle() {
	state = !state;
}

std::string Button::GetLabel() {
	return std::format("{} : Button", GetId());
}

std::string Button::getType() const {
	return "Button";
}

sf::FloatRect Button::getBounds() const {
	return sf::FloatRect(position, body.getSize());
}