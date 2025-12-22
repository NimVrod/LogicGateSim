#include "Component.h"

Component::Component(sf::Vector2f position)
    : m_position(position)
{
	id = nextId++;
}

void Component::draw(sf::RenderWindow& window) {
    // Base draw can handle pins
    for (const auto& pin : m_inputs) {
        pin->draw(window);
    }
    for (const auto& pin : m_outputs) {
        pin->draw(window);
    }
}

void Component::setPosition(sf::Vector2f pos) {
    m_position = pos;
}

sf::Vector2f Component::getPosition() const {
    return m_position;
}

const std::vector<std::unique_ptr<Pin>>& Component::getInputs() const {
    return m_inputs;
}

const std::vector<std::unique_ptr<Pin>>& Component::getOutputs() const {
    return m_outputs;
}

Pin* Component::addInput(sf::Vector2f relPos) {
    m_inputs.push_back(std::make_unique<Pin>(this, PinType::Input, relPos));
    return m_inputs.back().get();
}

Pin* Component::addOutput(sf::Vector2f relPos) {
    m_outputs.push_back(std::make_unique<Pin>(this, PinType::Output, relPos));
    return m_outputs.back().get();
}
