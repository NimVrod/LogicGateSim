#pragma once
#include "Component.h"
#include <memory>
#include <string>

class ComponentFactory {
public:
    static std::unique_ptr<Component> create(
        const std::string &typeName,
        int id,
        sf::Vector2f position,
        int numInputs = 2
    );

    static std::unique_ptr<Component> createInput(int id, sf::Vector2f position, int index);

    static std::unique_ptr<Component> createOutput(int id, sf::Vector2f position, int index);

    static std::unique_ptr<Component> createCustom(int id, sf::Vector2f position, const std::string &definitionName);
};