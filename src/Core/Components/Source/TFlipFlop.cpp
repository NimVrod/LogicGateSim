#include "../Include/TFlipFlop.h"
#include <format>
#include "../Include/ResourceManager.h"

TFlipFlop::TFlipFlop(int id, sf::Vector2f position)
    : FlipFlop(id, position, sf::Vector2f(60.f, 60.f)) {
    addInput(sf::Vector2f(0.f, 15.f));
    addInput(sf::Vector2f(0.f, 45.f));

    addOutput(sf::Vector2f(60.f, 20.f));
    addOutput(sf::Vector2f(60.f, 40.f));
}

void TFlipFlop::calculate() {
    if (inputs.size() < 2) return;

    bool t = inputs[0]->getValue() >= 1;
    bool clk = inputs[1]->getValue() >= 1;

    if (clk && !lastClk) {
        if (t) {
            q = !q;
        }
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

void TFlipFlop::drawPinLabels(sf::RenderTarget &target, sf::Vector2f pos) {
    ResourceManager &rm = ResourceManager::getInstance();
    sf::Font &font = rm.getDefaultFont();

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