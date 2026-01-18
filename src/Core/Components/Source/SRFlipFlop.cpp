#include "../Include/SRFlipFlop.h"
#include <format>
#include "../Include/ResourceManager.h"

SRFlipFlop::SRFlipFlop(int id, sf::Vector2f position)
    : FlipFlop(id, position, sf::Vector2f(60.f, 80.f)) {
    addInput(sf::Vector2f(0.f, 15.f));
    addInput(sf::Vector2f(0.f, 40.f));
    addInput(sf::Vector2f(0.f, 65.f));

    addOutput(sf::Vector2f(60.f, 25.f));
    addOutput(sf::Vector2f(60.f, 55.f));
}

void SRFlipFlop::calculate() {
    if (inputs.size() < 3) return;

    bool s = inputs[0]->getValue() >= 1;
    bool clk = inputs[1]->getValue() >= 1;
    bool r = inputs[2]->getValue() >= 1;

    if (clk && !lastClk) {
        if (s && !r) {
            q = true;
        } else if (!s && r) {
            q = false;
        }
    }

    lastClk = clk;

    if (outputs.size() >= 2) {
        outputs[0]->setValue(q ? 1 : 0);
        outputs[1]->setValue(q ? 0 : 1);
    }
}

std::string SRFlipFlop::GetLabel() {
    return std::format("{} : Przerzutnik SR", GetId());
}

std::string SRFlipFlop::getType() const {
    return "SRFlipFlop";
}

void SRFlipFlop::drawPinLabels(sf::RenderTarget &target, sf::Vector2f pos) {
    ResourceManager &rm = ResourceManager::getInstance();
    sf::Font &font = rm.getDefaultFont();

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