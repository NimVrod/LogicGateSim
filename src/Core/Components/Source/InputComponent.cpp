#include "../Include/InputComponent.h"
#include <format>
#include "Core/ResourceManager.h"

InputComponent::InputComponent(int id, sf::Vector2f position, int index)
    : Component(id, position), index(index), externalValue(0)
{
    // InputComponent has one output pin (provides value to the circuit)
    body.setFillColor(sf::Color(50, 150, 50));  // Green color for inputs
    body.setSize(sf::Vector2f(50.f, 30.f));
    body.setOutlineColor(sf::Color::White);
    body.setOutlineThickness(2.f);
    
    // Output pin on the right side
    addOutput(sf::Vector2f(50.f, 15.f));
}

void InputComponent::calculate() {
    // Pass through the external value to the output pin
    for (const auto& output : outputs) {
        output->setValue(externalValue);
    }
}

void InputComponent::setExternalValue(int value) {
    externalValue = value;
}

std::string InputComponent::GetLabel() {
    return std::format("{} : Input {}", GetId(), index);
}

std::string InputComponent::getType() const {
    return "InputComponent";
}

void InputComponent::draw(sf::RenderTarget& target) {
    sf::Vector2f pos = getPosition();
    
    // Draw the body
    body.setPosition(pos);
    target.draw(body);
    
    // Draw arrow pointing right (into the circuit)
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
    text.setString(std::format("In {}", index));
    text.setCharacterSize(12);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(pos.x + 5.f, pos.y + 7.f));
    target.draw(text);
}

void InputComponent::drawLabel(sf::RenderTarget& target) {
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getFont("assets/ARIAL.TTF");
    sf::Text text(font);
    text.setString(GetLabel());
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(position.x, position.y + body.getSize().y + 5.f));
    target.draw(text);
}

sf::FloatRect InputComponent::getBounds() const {
    return sf::FloatRect(position, body.getSize());
}
