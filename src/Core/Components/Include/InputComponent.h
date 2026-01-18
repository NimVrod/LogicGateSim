#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Component.h"

// InputComponent represents an input pin interface for custom components.
// When used in a sub-circuit, it receives values from external inputs.
class InputComponent : public Component {
public:
    InputComponent(int id, sf::Vector2f position, int index = 0);

    void calculate() override;

    std::string GetLabel() override;

    std::string getType() const override;

    void draw(sf::RenderTarget &target) override;

    void drawLabel(sf::RenderTarget &target) override;

    sf::FloatRect getBounds() const override;

    int getIndex() const { return index; }
    void setIndex(int idx) { index = idx; }

    // Set the value from external source (used by CustomComponent)
    void setExternalValue(int value);

protected:
    sf::RectangleShape body;

private:
    int index;
    int externalValue;
};