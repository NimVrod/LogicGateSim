#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <vector>
#include <memory>
#include <string>
#include <format>
#include "Pin.h"

class Component {
private:
    int id;
public:
    Component(int id, sf::Vector2f position);
    virtual ~Component() = default;

    virtual void calculate() = 0;
    virtual std::string GetLabel();
    virtual std::string getType() const;
	
    virtual void draw(sf::RenderTarget& target);
    virtual void drawPins(sf::RenderTarget& target);
    virtual void drawLabel(sf::RenderTarget& target);
    virtual sf::FloatRect getBounds() const;

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;

    const std::vector<std::unique_ptr<Pin>>& getInputs() const;
    const std::vector<std::unique_ptr<Pin>>& getOutputs() const;

    int GetId() const;

protected:
    sf::Vector2f position;
    std::vector<std::unique_ptr<Pin>> inputs;
    std::vector<std::unique_ptr<Pin>> outputs;

    Pin* addInput(sf::Vector2f relPos);
    Pin* addOutput(sf::Vector2f relPos);
};
