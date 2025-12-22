#include "Gates.h"

Gate::Gate(sf::Vector2f position, const std::string& label) 
    : Component(position) 
{
    m_body.setSize({60.f, 40.f});
    m_body.setOrigin({0.f, 0.f}); // Top-left is origin of shape, but our component origin handles position
    // Component position is top-left
    m_body.setPosition(position);
    m_body.setFillColor(sf::Color(100, 100, 150));
    m_body.setOutlineColor(sf::Color::White);
    m_body.setOutlineThickness(2.f);
}

void Gate::draw(sf::RenderWindow& window) {
    m_body.setPosition(getPosition());
    window.draw(m_body);
    Component::draw(window); // Draw pins
}

AndGate::AndGate(sf::Vector2f position)
    : Gate(position, "AND")
{
    m_body.setFillColor(sf::Color(0, 100, 100)); // Teal for AND
    addInput({0.f, 10.f});
    addInput({0.f, 30.f});
    addOutput({60.f, 20.f});
}

void AndGate::calculate() {
    bool a = m_inputs[0]->getValue() >= 1;
    bool b = m_inputs[1]->getValue() >= 1;
    m_outputs[0]->setValue(a && b ? 1 : 0);
}

OrGate::OrGate(sf::Vector2f position)
    : Gate(position, "OR")
{
    m_body.setFillColor(sf::Color(100, 0, 100)); // Purple for OR
    addInput({0.f, 10.f});
    addInput({0.f, 30.f});
    addOutput({60.f, 20.f});
}

void OrGate::calculate() {
    bool a = m_inputs[0]->getValue() >= 1;
    bool b = m_inputs[1]->getValue() >= 1;
    m_outputs[0]->setValue(a || b ? 1 : 0);
}

NotGate::NotGate(sf::Vector2f position)
    : Gate(position, "NOT")
{
    m_body.setFillColor(sf::Color(100, 0, 0)); // Red for NOT
    // Triangle shape usually, but rect is fine
    m_body.setSize({60.f, 40.f}); // Keep uniform size or make it smaller
    
    addInput({0.f, 20.f});
    addOutput({60.f, 20.f});
}

void NotGate::calculate() {
    bool a = m_inputs[0]->getValue() >= 1;
    m_outputs[0]->setValue(!a ? 1 : 0);
}
