#include "../Include/JKFlipFlop.h"
#include <format>
#include "../Include/ResourceManager.h"

JKFlipFlop::JKFlipFlop(int id, sf::Vector2f position)
    : FlipFlop(id, position, sf::Vector2f(60.f, 80.f))
{
    addInput(sf::Vector2f(0.f, 15.f));
    addInput(sf::Vector2f(0.f, 40.f));
    addInput(sf::Vector2f(0.f, 65.f));
    
    addOutput(sf::Vector2f(60.f, 25.f));
    addOutput(sf::Vector2f(60.f, 55.f));
}

void JKFlipFlop::calculate() {
    if (inputs.size() < 3) return;
    
    bool j = inputs[0]->getValue() >= 1;
    bool clk = inputs[1]->getValue() >= 1;
    bool k = inputs[2]->getValue() >= 1;
    
    if (clk && !lastClk) {
        if (j && !k) {
            q = true;
        } else if (!j && k) {
            q = false;
        } else if (j && k) {
            q = !q;
        }
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

void JKFlipFlop::drawPinLabels(sf::RenderTarget& target, sf::Vector2f pos) {
    ResourceManager& rm = ResourceManager::getInstance();
    sf::Font& font = rm.getDefaultFont();
    
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
