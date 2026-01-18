#include "../Include/ComponentFactory.h"
#include "../Include/Button.h"
#include "../Include/AndGate.h"
#include "../Include/OrGate.h"
#include "../Include/NotGate.h"
#include "../Include/NandGate.h"
#include "../Include/NorGate.h"
#include "../Include/XorGate.h"
#include "../Include/XnorGate.h"
#include "../Include/InputComponent.h"
#include "../Include/OutputComponent.h"
#include "../CustomComponent/CustomComponent.h"
#include "../Include/ClockComponent.h"
#include "../Include/LEDComponent.h"
#include "../Include/SRFlipFlop.h"
#include "../Include/DFlipFlop.h"
#include "../Include/JKFlipFlop.h"
#include <unordered_map>
#include <functional>
#include "Core/Components/Include/TFlipFlop.h"

std::unique_ptr<Component> ComponentFactory::create(
    const std::string &typeName,
    int id,
    sf::Vector2f position,
    int numInputs) {
    static const std::unordered_map<std::string, std::function<std::unique_ptr<Component>(int, sf::Vector2f, int)> >
            factoryMap = {
                {"Button", [](int id, sf::Vector2f pos, int) { return std::make_unique<Button>(id, pos); }},
                {"AndGate", [](int id, sf::Vector2f pos, int n) { return std::make_unique<AndGate>(id, pos, n); }},
                {"OrGate", [](int id, sf::Vector2f pos, int n) { return std::make_unique<OrGate>(id, pos, n); }},
                {"NotGate", [](int id, sf::Vector2f pos, int) { return std::make_unique<NotGate>(id, pos); }},
                {"NandGate", [](int id, sf::Vector2f pos, int n) { return std::make_unique<NandGate>(id, pos, n); }},
                {"NorGate", [](int id, sf::Vector2f pos, int n) { return std::make_unique<NorGate>(id, pos, n); }},
                {"XorGate", [](int id, sf::Vector2f pos, int n) { return std::make_unique<XorGate>(id, pos, n); }},
                {"XnorGate", [](int id, sf::Vector2f pos, int n) { return std::make_unique<XnorGate>(id, pos, n); }},
                {
                    "ClockComponent",
                    [](int id, sf::Vector2f pos, int) { return std::make_unique<ClockComponent>(id, pos); }
                },
                {"LEDComponent", [](int id, sf::Vector2f pos, int) { return std::make_unique<LEDComponent>(id, pos); }},
                {"SRFlipFlop", [](int id, sf::Vector2f pos, int) { return std::make_unique<SRFlipFlop>(id, pos); }},
                {"DFlipFlop", [](int id, sf::Vector2f pos, int) { return std::make_unique<DFlipFlop>(id, pos); }},
                {"JKFlipFlop", [](int id, sf::Vector2f pos, int) { return std::make_unique<JKFlipFlop>(id, pos); }},
                {"TFlipFlop", [](int id, sf::Vector2f pos, int) { return std::make_unique<TFlipFlop>(id, pos); }},
                {
                    "Input Pin",
                    [](int id, sf::Vector2f pos, int index) { return std::make_unique<InputComponent>(id, pos, index); }
                },
                {
                    "Output Pin",
                    [](int id, sf::Vector2f pos, int index) {
                        return std::make_unique<OutputComponent>(id, pos, index);
                    }
                }
            };

    auto it = factoryMap.find(typeName);
    if (it != factoryMap.end()) {
        return it->second(id, position, numInputs);
    }
    return createCustom(id, position, typeName);
}

std::unique_ptr<Component> ComponentFactory::createInput(int id, sf::Vector2f position, int index) {
    return std::make_unique<InputComponent>(id, position, index);
}

std::unique_ptr<Component> ComponentFactory::createOutput(int id, sf::Vector2f position, int index) {
    return std::make_unique<OutputComponent>(id, position, index);
}

std::unique_ptr<Component> ComponentFactory::createCustom(int id, sf::Vector2f position,
                                                          const std::string &definitionName) {
    return std::make_unique<CustomComponent>(id, position, definitionName);
}
