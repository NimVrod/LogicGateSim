#include "../Include/FlipFlop.h"
#include "../Include/ResourceManager.h"

FlipFlop::FlipFlop(int id, sf::Vector2f position, sf::Vector2f bodySize)
    : Component(id, position), q(false), lastClk(false) {
    body.setFillColor(sf::Color(120, 80, 120));
    body.setSize(bodySize);
    body.setOutlineColor(sf::Color::White);
    body.setOutlineThickness(2.f);
}

void FlipFlop::draw(sf::RenderTarget &target) {
    sf::Vector2f pos = getPosition();

    if (q) {
        body.setFillColor(sf::Color(160, 100, 160));
    } else {
        body.setFillColor(sf::Color(100, 60, 100));
    }

    body.setPosition(pos);
    target.draw(body);

    ResourceManager &rm = ResourceManager::getInstance();
    sf::Font &font = rm.getDefaultFont();

    sf::Text title(font);
    title.setString(getTitle());
    title.setCharacterSize(14);
    title.setFillColor(sf::Color::White);

    sf::FloatRect titleBounds = title.getLocalBounds();
    float titleX = pos.x + (body.getSize().x - titleBounds.size.x) / 2.f;
    float titleY = pos.y + (body.getSize().y - titleBounds.size.y) / 2.f - 5.f;
    title.setPosition(sf::Vector2f(titleX, titleY));
    target.draw(title);

    drawPinLabels(target, pos);
}

void FlipFlop::drawLabel(sf::RenderTarget &target) {
    ResourceManager &rm = ResourceManager::getInstance();
    sf::Font &font = rm.getDefaultFont();
    sf::Text text(font);
    text.setString(GetLabel());
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(position.x, position.y + body.getSize().y + 5.f));
    target.draw(text);
}

sf::FloatRect FlipFlop::getBounds() const {
    return sf::FloatRect(position, body.getSize());
}