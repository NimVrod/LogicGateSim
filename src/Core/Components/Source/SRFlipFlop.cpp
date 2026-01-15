#include "../Include/SRFlipFlop.h"
#include <format>
#include "Core/ResourceManager.h"

SRFlipFlop::SRFlipFlop(int id, sf::Vector2f position)
    : Component(id, position), q(false), lastClk(false)
{
    body.setFillColor(sf::Color(120, 80, 120));  // Purple for flip flops
    body.setSize(sf::Vector2f(60.f, 80.f));
    body.setOutlineColor(sf::Color::White);
    body.setOutlineThickness(2.f);
    
    // Inputs: S, CLK, R (top to bottom on left)
    addInput(sf::Vector2f(0.f, 15.f));   // S (Set)
    addInput(sf::Vector2f(0.f, 40.f));   // CLK (Clock)
    addInput(sf::Vector2f(0.f, 65.f));   // R (Reset)
    
    // Outputs: Q, Q' (top to bottom on right)
    addOutput(sf::Vector2f(60.f, 25.f)); // Q
    addOutput(sf::Vector2f(60.f, 55.f)); // Q' (inverted)
}

void SRFlipFlop::calculate() {
    if (inputs.size() < 3) return;
    
    bool s = inputs[0]->getValue() >= 1;
    bool clk = inputs[1]->getValue() >= 1;
    bool r = inputs[2]->getValue() >= 1;
    
    // Rising edge detection
    if (clk && !lastClk) {
        if (s && !r) {
            q = true;
        } else if (!s && r) {
            q = false;
        }
        // s && r is undefined - we ignore it
    }
    
    lastClk = clk;
    
    // Set outputs
    if (outputs.size() >= 2) {
        outputs[0]->setValue(q ? 1 : 0);      // Q
        outputs[1]->setValue(q ? 0 : 1);      // Q' (inverted)
    }
}

std::string SRFlipFlop::GetLabel() {
    return std::format("{} : SR FF", GetId());
}

std::string SRFlipFlop::getType() const {
    return "SRFlipFlop";
}

void SRFlipFlop::draw(sf::RenderTarget& target) {
    sf::Vector2f pos = getPosition();
    
    // Change color based on state
    if (q) {
        body.setFillColor(sf::Color(160, 100, 160));
    } else {
        body.setFillColor(sf::Color(100, 60, 100));
    }
    
    body.setPosition(pos);
    target.draw(body);
    
    // Draw labels
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getFont("assets/ARIAL.TTF");
    
    // Title
    sf::Text title(font);
    title.setString("SR");
    title.setCharacterSize(14);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(pos.x + 20.f, pos.y + 30.f));
    target.draw(title);
    
    // Pin labels
    sf::Text pinLabel(font);
    pinLabel.setCharacterSize(10);
    pinLabel.setFillColor(sf::Color::White);
    
    pinLabel.setString("S");
    pinLabel.setPosition(sf::Vector2f(pos.x + 5.f, pos.y + 8.f));
    target.draw(pinLabel);
    
    pinLabel.setString(">");
    pinLabel.setPosition(sf::Vector2f(pos.x + 5.f, pos.y + 33.f));
    target.draw(pinLabel);
    
    pinLabel.setString("R");
    pinLabel.setPosition(sf::Vector2f(pos.x + 5.f, pos.y + 58.f));
    target.draw(pinLabel);
    
    pinLabel.setString("Q");
    pinLabel.setPosition(sf::Vector2f(pos.x + 48.f, pos.y + 18.f));
    target.draw(pinLabel);
    
    pinLabel.setString("Q'");
    pinLabel.setPosition(sf::Vector2f(pos.x + 45.f, pos.y + 48.f));
    target.draw(pinLabel);
}

void SRFlipFlop::drawLabel(sf::RenderTarget& target) {
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getFont("assets/ARIAL.TTF");
    sf::Text text(font);
    text.setString(GetLabel());
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(position.x, position.y + body.getSize().y + 5.f));
    target.draw(text);
}

sf::FloatRect SRFlipFlop::getBounds() const {
    return sf::FloatRect(position, body.getSize());
}
