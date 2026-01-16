#pragma once

#include "Component.h"
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>

class Gate : public Component {
public:
	Gate(int id, sf::Vector2f position, int numInputs = 2);
	virtual void calculate() = 0;
	sf::FloatRect getBounds() const override;

protected:
	virtual std::string GetLabel() override;
	sf::RectangleShape body;
};