#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "../../Component.h"

class Button : public Component {
	public:
	Button(int id, sf::Vector2f position, int output = 1);
	void calculate() override;
	std::string GetLabel() override;
	void draw(sf::RenderWindow& window) override;
	void drawLabel(sf::RenderWindow &window) override;
	sf::FloatRect getBounds() const override;
	void toggle();
protected:
	sf::RectangleShape body;
private:
	bool state;
	float output;
};