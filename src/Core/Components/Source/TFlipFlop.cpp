#include "../Include/TFlipFlop.h"
#include <format>
#include "Core/ResourceManager.h"

TFlipFlop::TFlipFlop(int id, sf::Vector2f position)
    : Component(id, position), q(false), lastClk(false)
{
    body.setFillColor(sf::Color(120, 80, 120));
    body.setSize(sf::Vector2f(60.f, 60.f));
    body.setOutlineColor(sf::Color::White);
    body.setOutlineThickness(2.f);
    
    // Inputs: T, CLK
    addInput(sf::Vector2f(0.f, 15.f));   // T
    addInput(sf::Vector2f(0.f, 45.f));   // CLK
    
    // Outputs: Q, Q'
    addOutput(sf::Vector2f(60.f, 20.f)); // Q
    addOutput(sf::Vector2f(60.f, 40.f)); // Q'
}

void TFlipFlop::calculate() {
    if (inputs.size() < 2) return;
    
    bool t = inputs[0]->getValue() >= 1;
    bool clk = inputs[1]->getValue() >= 1;
    
    // Rising edge detection
    if (clk && !lastClk) {
        if (t) {
            q = !q;  // Toggle if T is high
        }
        // T=0 - hold state
    }
    
    lastClk = clk;
    
    if (outputs.size() >= 2) {
        outputs[0]->setValue(q ? 1 : 0);
        outputs[1]->setValue(q ? 0 : 1);
    }
}

std::string TFlipFlop::GetLabel() {
    return std::format("{} : T FF", GetId());
}

std::string TFlipFlop::getType() const {
    return "TFlipFlop";
}

void TFlipFlop::draw(sf::RenderTarget& target) {
    sf::Vector2f pos = getPosition();
    
    if (q) {
        body.setFillColor(sf::Color(160, 100, 160));
    } else {
        body.setFillColor(sf::Color(100, 60, 100));
    }
    
    body.setPosition(pos);
    target.draw(body);
    
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getFont("assets/ARIAL.TTF");
    
    sf::Text title(font);
    title.setString("T");
    title.setCharacterSize(16);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(pos.x + 25.f, pos.y + 18.f));
    target.draw(title);
    
    sf::Text pinLabel(font);
    pinLabel.setCharacterSize(10);
    pinLabel.setFillColor(sf::Color::White);
    
    pinLabel.setString("T");
    pinLabel.setPosition(sf::Vector2f(pos.x + 5.f, pos.y + 8.f));
    target.draw(pinLabel);
    
    pinLabel.setString(">");
    pinLabel.setPosition(sf::Vector2f(pos.x + 5.f, pos.y + 38.f));
    target.draw(pinLabel);
    
    pinLabel.setString("Q");
    pinLabel.setPosition(sf::Vector2f(pos.x + 48.f, pos.y + 13.f));
    target.draw(pinLabel);
    
    pinLabel.setString("Q'");
    pinLabel.setPosition(sf::Vector2f(pos.x + 45.f, pos.y + 33.f));
    target.draw(pinLabel);
}

void TFlipFlop::drawLabel(sf::RenderTarget& target) {
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getFont("assets/ARIAL.TTF");
    sf::Text text(font);
    text.setString(GetLabel());
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(position.x, position.y + body.getSize().y + 5.f));
    target.draw(text);
}

sf::FloatRect TFlipFlop::getBounds() const {
    return sf::FloatRect(position, body.getSize());
}
