#include "../Include/InputComponent.h"
#include <format>
#include "../Include/ResourceManager.h"

InputComponent::InputComponent(int id, sf::Vector2f position, int index)
    : Component(id, position), index(index), externalValue(0) {
    body.setFillColor(sf::Color(50, 150, 50));
    body.setSize(sf::Vector2f(50.f, 30.f));
    body.setOutlineColor(sf::Color::White);
    body.setOutlineThickness(2.f);

    addOutput(sf::Vector2f(50.f, 15.f));
}

void InputComponent::calculate() {
    for (const auto &output: outputs) {
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
    return "Input Pin";
}

void InputComponent::draw(sf::RenderTarget &target) {
    sf::Vector2f pos = getPosition();

    body.setPosition(pos);
    target.draw(body);

    sf::ConvexShape arrow;
    arrow.setPointCount(3);
    arrow.setPoint(0, sf::Vector2f(0.f, 0.f));
    arrow.setPoint(1, sf::Vector2f(0.f, 10.f));
    arrow.setPoint(2, sf::Vector2f(8.f, 5.f));
    arrow.setPosition(sf::Vector2f(pos.x + 38.f, pos.y + 10.f));
    arrow.setFillColor(sf::Color::White);
    target.draw(arrow);

    ResourceManager &rm = ResourceManager::getInstance();
    sf::Font &font = rm.getDefaultFont();
    sf::Text text(font);
    text.setString(std::format("In {}", index));
    text.setCharacterSize(12);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(pos.x + 5.f, pos.y + 7.f));
    target.draw(text);
}

void InputComponent::drawLabel(sf::RenderTarget &target) {
    ResourceManager &rm = ResourceManager::getInstance();
    sf::Font &font = rm.getDefaultFont();
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