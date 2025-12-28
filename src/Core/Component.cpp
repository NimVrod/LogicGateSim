#include "Component.h"

#include "ResourceManager.h"
#include "SFML/Graphics/Text.hpp"

int Component::nextId = 1;

Component::Component(sf::Vector2f position)
    : position(position)
{
	id = nextId++;
}

void Component::draw(sf::RenderWindow& window) {
    // Base draw - subclasses handle their own drawing
}

void Component::drawPins(sf::RenderWindow &window) {
    for (const auto& pin : inputs) {
        pin->draw(window);
    }
    for (const auto& pin : outputs) {
        pin->draw(window);
    }
}

void Component::drawLabel(sf::RenderWindow &window) {
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getFont("assets/ARIAL.TTF");
    sf::Text text(font);
    text.setString(GetLabel());
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    auto longer = std::max(getInputs().size(), getOutputs().size());
    text.setPosition(sf::Vector2f(position.x - 10.f, position.y + longer * 20.f + 10.f ));
    window.draw(text);
}

void Component::setPosition(sf::Vector2f pos) {
    position = pos;
}

sf::Vector2f Component::getPosition() const {
    return position;
}

sf::FloatRect Component::getBounds() const {
    return sf::FloatRect(position, sf::Vector2f(60.f, 40.f));
}

const std::vector<std::unique_ptr<Pin>>& Component::getInputs() const {
    return inputs;
}

const std::vector<std::unique_ptr<Pin>>& Component::getOutputs() const {
    return outputs;
}

Pin* Component::addInput(sf::Vector2f relPos) {
    inputs.push_back(std::make_unique<Pin>(this, PinType::Input, relPos));
    return inputs.back().get();
}

Pin* Component::addOutput(sf::Vector2f relPos) {
    outputs.push_back(std::make_unique<Pin>(this, PinType::Output, relPos));
    return outputs.back().get();
}

std::string Component::GetLabel() {
    return std::format("{} : Component", GetId());
}

int Component::GetId() {
    return id;
}
