#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include <memory>
#include "Pin.h"

class Component {
public:
    Component(sf::Vector2f position);
    virtual ~Component() = default;

    virtual void calculate() = 0;
    virtual void draw(sf::RenderWindow& window);

    void setPosition(sf::Vector2f pos);
    sf::Vector2f getPosition() const;

    const std::vector<std::unique_ptr<Pin>>& getInputs() const;
    const std::vector<std::unique_ptr<Pin>>& getOutputs() const;

protected:
    static int nextId;
	int id;
    std::string label;
    sf::Vector2f m_position;
    std::vector<std::unique_ptr<Pin>> m_inputs;
    std::vector<std::unique_ptr<Pin>> m_outputs;

    Pin* addInput(sf::Vector2f relPos);
    Pin* addOutput(sf::Vector2f relPos);
};
