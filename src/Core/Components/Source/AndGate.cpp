#include "../include/AndGate.h"
#include <SFML/Graphics/ConvexShape.hpp>
#include <cmath>

AndGate::AndGate(int id, sf::Vector2f position, int numInputs)
	: Gate(id, position, numInputs)
{
	body.setFillColor(sf::Color::Transparent);
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
	sf::ConvexShape shape;
	
	float w = 60.f;
	float h = 0.f;
	if (!inputs.empty()) {
	    // Calculate height based on inputs to match pins naturally
        // Pin layout: 10 + i*20. Last pin at 10 + (n-1)*20.
        // Height needed is roughly ((n-1)*20 + 20) = n*20.
        h = inputs.size() * 20.f; 
	} else {
	    h = 40.f;
	}
    // Min height 40 for shape look
    if (h < 40.f) h = 40.f;

	// Refine height to match body size if set by Gate
	// body size is 60 x numInputs*20.
    h = body.getSize().y;

	sf::Vector2f pos = getPosition();

	// Generate standard AND shape (D-shape)
	// Flat back at x=0
	// Flat top/bottom for first half
	// Curved front
	
	// Define points
	std::vector<sf::Vector2f> points;
	
	float curveStart = w * 0.5f; // 30.f
	
	// Top Left
	points.push_back(sf::Vector2f(0.f, 0.f));
	
	// Curve
	int segments = 20;
	for (int i = 0; i <= segments; ++i) {
	    float angle = -90.f + (180.f * i / segments); // -90 to 90
	    float rad = angle * 3.14159f / 180.f;
	    
	    // Ellipse centered at (curveStart, h/2)
	    // Radius X = w - curveStart = 30
	    // Radius Y = h/2
	    
	    float rx = w - curveStart;
	    float ry = h / 2.f;
	    
	    float x = curveStart + rx * std::cos(rad);
	    float y = h / 2.f + ry * std::sin(rad);
	    
	    points.push_back(sf::Vector2f(x, y));
	}
	
	// Bottom Left
	points.push_back(sf::Vector2f(0.f, h));
	
	shape.setPointCount(points.size());
	for (size_t i = 0; i < points.size(); ++i) {
	    shape.setPoint(i, points[i]);
	}
	
	shape.setPosition(pos);
	shape.setFillColor(sf::Color::Transparent); // Original bluish color
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(2.f);
	shape.setOutlineColor(sf::Color::White);
	window.draw(shape);
}