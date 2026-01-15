#include "../Include/JKFlipFlop.h"
#include <format>
#include "Core/ResourceManager.h"

JKFlipFlop::JKFlipFlop(int id, sf::Vector2f position)
    : Component(id, position), q(false), lastClk(false)
{
    body.setFillColor(sf::Color(120, 80, 120));
    body.setSize(sf::Vector2f(60.f, 80.f));
    body.setOutlineColor(sf::Color::White);
    body.setOutlineThickness(2.f);
    
    // Inputs: J, CLK, K
    addInput(sf::Vector2f(0.f, 15.f));   // J
    addInput(sf::Vector2f(0.f, 40.f));   // CLK
    addInput(sf::Vector2f(0.f, 65.f));   // K
    
    // Outputs: Q, Q'
    addOutput(sf::Vector2f(60.f, 25.f)); // Q
    addOutput(sf::Vector2f(60.f, 55.f)); // Q'
}

void JKFlipFlop::calculate() {
    if (inputs.size() < 3) return;
    
    bool j = inputs[0]->getValue() >= 1;
    bool clk = inputs[1]->getValue() >= 1;
    bool k = inputs[2]->getValue() >= 1;
    
    // Rising edge detection
    if (clk && !lastClk) {
        if (j && !k) {
            q = true;
        } else if (!j && k) {
            q = false;
        } else if (j && k) {
            q = !q;  // Toggle
        }
        // !j && !k - hold state (do nothing)
    }
    
    lastClk = clk;
    
    if (outputs.size() >= 2) {
        outputs[0]->setValue(q ? 1 : 0);
        outputs[1]->setValue(q ? 0 : 1);
    }
}

std::string JKFlipFlop::GetLabel() {
    return std::format("{} : JK FF", GetId());
}

std::string JKFlipFlop::getType() const {
    return "JKFlipFlop";
}

void JKFlipFlop::draw(sf::RenderTarget& target) {
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
    title.setString("JK");
    title.setCharacterSize(14);
    title.setFillColor(sf::Color::White);
    title.setPosition(sf::Vector2f(pos.x + 20.f, pos.y + 30.f));
    target.draw(title);
    
    sf::Text pinLabel(font);
    pinLabel.setCharacterSize(10);
    pinLabel.setFillColor(sf::Color::White);
    
    pinLabel.setString("J");
    pinLabel.setPosition(sf::Vector2f(pos.x + 5.f, pos.y + 8.f));
    target.draw(pinLabel);
    
    pinLabel.setString(">");
    pinLabel.setPosition(sf::Vector2f(pos.x + 5.f, pos.y + 33.f));
    target.draw(pinLabel);
    
    pinLabel.setString("K");
    pinLabel.setPosition(sf::Vector2f(pos.x + 5.f, pos.y + 58.f));
    target.draw(pinLabel);
    
    pinLabel.setString("Q");
    pinLabel.setPosition(sf::Vector2f(pos.x + 48.f, pos.y + 18.f));
    target.draw(pinLabel);
    
    pinLabel.setString("Q'");
    pinLabel.setPosition(sf::Vector2f(pos.x + 45.f, pos.y + 48.f));
    target.draw(pinLabel);
}

void JKFlipFlop::drawLabel(sf::RenderTarget& target) {
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getFont("assets/ARIAL.TTF");
    sf::Text text(font);
    text.setString(GetLabel());
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(position.x, position.y + body.getSize().y + 5.f));
    target.draw(text);
}

sf::FloatRect JKFlipFlop::getBounds() const {
    return sf::FloatRect(position, body.getSize());
}
