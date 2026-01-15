#include "../Include/OutputComponent.h"
#include <format>
#include "Core/ResourceManager.h"

OutputComponent::OutputComponent(int id, sf::Vector2f position, int index)
    : Component(id, position), index(index), outputValue(0)
{
    // OutputComponent has one input pin (receives value from the circuit)
    body.setFillColor(sf::Color(150, 50, 50));  // Red color for outputs
    body.setSize(sf::Vector2f(50.f, 30.f));
    body.setOutlineColor(sf::Color::White);
    body.setOutlineThickness(2.f);
    
    // Input pin on the left side
    addInput(sf::Vector2f(0.f, 15.f));
}

void OutputComponent::calculate() {
    // Store the input value for CustomComponent to read
    if (!inputs.empty()) {
        outputValue = inputs[0]->getValue();
    }
}

std::string OutputComponent::GetLabel() {
    return std::format("{} : Output {}", GetId(), index);
}

std::string OutputComponent::getType() const {
    return "OutputComponent";
}

void OutputComponent::draw(sf::RenderTarget& target) {
    sf::Vector2f pos = getPosition();
    
    // Draw the body
    body.setPosition(pos);
    target.draw(body);
    
    // Draw arrow pointing right (out of the circuit)
    sf::ConvexShape arrow;
    arrow.setPointCount(3);
    arrow.setPoint(0, sf::Vector2f(0.f, 0.f));
    arrow.setPoint(1, sf::Vector2f(0.f, 10.f));
    arrow.setPoint(2, sf::Vector2f(8.f, 5.f));
    arrow.setPosition(sf::Vector2f(pos.x + 38.f, pos.y + 10.f));
    arrow.setFillColor(sf::Color::White);
    target.draw(arrow);
    
    // Draw the index label inside the box
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getFont("assets/ARIAL.TTF");
    sf::Text text(font);
    text.setString(std::format("Out {}", index));
    text.setCharacterSize(12);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(pos.x + 5.f, pos.y + 7.f));
    target.draw(text);
}

void OutputComponent::drawLabel(sf::RenderTarget& target) {
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getFont("assets/ARIAL.TTF");
    sf::Text text(font);
    text.setString(GetLabel());
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(position.x, position.y + body.getSize().y + 5.f));
    target.draw(text);
}

sf::FloatRect OutputComponent::getBounds() const {
    return sf::FloatRect(position, body.getSize());
}
