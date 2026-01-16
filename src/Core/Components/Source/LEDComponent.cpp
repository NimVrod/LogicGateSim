#include "../Include/LEDComponent.h"
#include <format>
#include "../Include/ResourceManager.h"

LEDComponent::LEDComponent(int id, sf::Vector2f position, sf::Color onColor)
    : Component(id, position), lit(false), onColor(onColor), offColor(sf::Color(60, 60, 60))
{
    body.setRadius(15.f);
    body.setFillColor(offColor);
    body.setOutlineColor(sf::Color::White);
    body.setOutlineThickness(2.f);
    
    addInput(sf::Vector2f(0.f, 15.f));
}

void LEDComponent::calculate() {
    if (!inputs.empty()) {
        lit = inputs[0]->getValue() >= 1;
    } else {
        lit = false;
    }
}

std::string LEDComponent::GetLabel() {
    return std::format("{} : LED", GetId());
}

std::string LEDComponent::getType() const {
    return "LEDComponent";
}

void LEDComponent::draw(sf::RenderTarget& target) {
    sf::Vector2f pos = getPosition();
    
    if (lit) {
        body.setFillColor(onColor);
    } else {
        body.setFillColor(offColor);
    }
    
    body.setPosition(pos);
    target.draw(body);
}

void LEDComponent::drawLabel(sf::RenderTarget& target) {
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getDefaultFont();
    sf::Text text(font);
    text.setString(GetLabel());
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(position.x, position.y + body.getRadius() * 2.f + 5.f));
    target.draw(text);
}

sf::FloatRect LEDComponent::getBounds() const {
    return sf::FloatRect(position, sf::Vector2f(body.getRadius() * 2.f, body.getRadius() * 2.f));
}
