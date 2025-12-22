#include "IOComponents.h"

Button::Button(sf::Vector2f position)
    : Component(position), m_state(false)
{
    m_body.setSize({40.f, 40.f});
    m_body.setPosition(position);
    m_body.setFillColor(sf::Color(50, 50, 150));
    m_body.setOutlineColor(sf::Color::White);
    m_body.setOutlineThickness(2.f);

    addOutput({40.f, 20.f});
}

void Button::calculate() {
    m_outputs[0]->setValue(m_state ? 1 : 0);
}

void Button::toggle() {
    m_state = !m_state;
    m_body.setFillColor(m_state ? sf::Color(100, 100, 250) : sf::Color(50, 50, 150));
}

bool Button::isMouseOver(sf::Vector2f mousePos) const {
    // Check against current position
    sf::FloatRect bounds = m_body.getLocalBounds();
    bounds.position += getPosition();
    return bounds.contains(mousePos);
}

void Button::draw(sf::RenderWindow& window) {
    m_body.setPosition(getPosition());
    window.draw(m_body);
    Component::draw(window);
}

Led::Led(sf::Vector2f position)
    : Component(position)
{
    m_offColor = sf::Color(50, 0, 0);
    m_onColor = sf::Color(255, 0, 0);

    m_body.setRadius(15.f);
    m_body.setOrigin({15.f, 15.f});
    // Visual body is offset, input pin is at 0,20
    
    addInput({0.f, 20.f});
}

void Led::calculate() {
    bool on = m_inputs[0]->getValue() >= 1;
    m_body.setFillColor(on ? m_onColor : m_offColor);
}

void Led::draw(sf::RenderWindow& window) {
    // Draw body centered roughly after the input pin
    m_body.setPosition(getPosition() + sf::Vector2f(25.f, 20.f)); 
    window.draw(m_body);
    Component::draw(window);
}
