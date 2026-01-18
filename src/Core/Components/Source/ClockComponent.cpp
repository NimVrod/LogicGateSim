#include "../Include/ClockComponent.h"
#include <format>
#include "../Include/ResourceManager.h"

ClockComponent::ClockComponent(int id, sf::Vector2f position, float frequency)
    : Component(id, position), state(false), frequency(frequency), accumulator(0.0f) {
    body.setFillColor(sf::Color(100, 100, 200));
    body.setSize(sf::Vector2f(50.f, 40.f));
    body.setOutlineColor(sf::Color::White);
    body.setOutlineThickness(2.f);

    addOutput(sf::Vector2f(50.f, 20.f));
}

void ClockComponent::tick() {
    state = !state;
}

void ClockComponent::calculate() {
    for (const auto &output: outputs) {
        output->setValue(state ? 1 : 0);
    }
}

std::string ClockComponent::GetLabel() {
    return std::format("{} : Zegar", GetId());
}

std::string ClockComponent::getType() const {
    return "ClockComponent";
}

void ClockComponent::draw(sf::RenderTarget &target) {
    sf::Vector2f pos = getPosition();

    if (state) {
        body.setFillColor(sf::Color(150, 150, 255));
    } else {
        body.setFillColor(sf::Color(80, 80, 180));
    }

    body.setPosition(pos);
    target.draw(body);

    ResourceManager &rm = ResourceManager::getInstance();
    sf::Font &font = rm.getDefaultFont();
    sf::Text text(font);
    text.setString("CLK");
    text.setCharacterSize(12);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(pos.x + 10.f, pos.y + 12.f));
    target.draw(text);
}

void ClockComponent::drawLabel(sf::RenderTarget &target) {
    ResourceManager &rm = ResourceManager::getInstance();
    sf::Font &font = rm.getDefaultFont();
    sf::Text text(font);
    text.setString(GetLabel());
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(position.x, position.y + body.getSize().y + 5.f));
    target.draw(text);
}

sf::FloatRect ClockComponent::getBounds() const {
    return sf::FloatRect(position, body.getSize());
}