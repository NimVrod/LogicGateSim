#include "Component.h"

int Component::nextId = 0;

Component::Component(sf::Vector2f position)
    : m_position(position)
{
	id = nextId++;
}

void Component::draw(sf::RenderWindow& window, bool shouldDrawPins) {
    // Base draw can handle pins
    if (shouldDrawPins) {
        drawPins(window);
    }
}

void Component::drawPins(sf::RenderWindow &window) {
    for (const auto& pin : inputs) {
        pin->draw(window);
    }
    for (const auto& pin : outputs) {
        pin->draw(window);
    }
}

void Component::setPosition(sf::Vector2f pos) {
    m_position = pos;
}

sf::Vector2f Component::getPosition() const {
    return m_position;
}

sf::FloatRect Component::getBounds() const {
    return sf::FloatRect(m_position, sf::Vector2f(60.f, 40.f));
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
