#include "CustomComponent.h"
#include "CustomComponentManager.h"
#include "CustomComponentDefinition.h"
#include "../../../Simulation/Circuit.h"
#include "../../../Serialization/CircuitSerializer.h"
#include "../Include/InputComponent.h"
#include "../Include/OutputComponent.h"
#include "../Include/ResourceManager.h"
#include <format>
#include <algorithm>

CustomComponent::CustomComponent(int id, sf::Vector2f position, const std::string &definitionName)
    : Component(id, position), definitionName(definitionName), internalCircuit(nullptr) {
    const auto *def = CustomComponentManager::getInstance().getDefinition(definitionName);
    if (def) {
        initFromDefinition(def);
    }

    body.setFillColor(sf::Color(80, 80, 120));
    body.setOutlineColor(sf::Color::White);
    body.setOutlineThickness(2.f);
}

CustomComponent::~CustomComponent() = default;

void CustomComponent::initFromDefinition(const CustomComponentDefinition *def) {
    if (!def) return;

    internalCircuit = std::make_unique<Circuit>();

    if (!CircuitSerializer::loadFromXmlString(*internalCircuit, def->circuitXml)) {
        internalCircuit.reset();
        return;
    }

    cacheInternalComponents();

    int maxPins = std::max(static_cast<int>(internalInputs.size()),
                           static_cast<int>(internalOutputs.size()));
    maxPins = std::max(maxPins, 1);

    float height = maxPins * 25.f + 10.f;
    float width = 80.f;
    body.setSize(sf::Vector2f(width, height));

    for (size_t i = 0; i < internalInputs.size(); ++i) {
        float yOffset = 15.f + i * 25.f;
        addInput(sf::Vector2f(0.f, yOffset));
    }

    for (size_t i = 0; i < internalOutputs.size(); ++i) {
        float yOffset = 15.f + i * 25.f;
        addOutput(sf::Vector2f(width, yOffset));
    }
}

void CustomComponent::cacheInternalComponents() {
    if (!internalCircuit) return;

    internalInputs.clear();
    internalOutputs.clear();

    for (const auto &comp: internalCircuit->GetComponents()) {
        if (auto *input = dynamic_cast<InputComponent *>(comp.get())) {
            internalInputs.push_back(input);
        } else if (auto *output = dynamic_cast<OutputComponent *>(comp.get())) {
            internalOutputs.push_back(output);
        }
    }

    std::sort(internalInputs.begin(), internalInputs.end(),
              [](InputComponent *a, InputComponent *b) {
                  return a->getIndex() < b->getIndex();
              });

    std::sort(internalOutputs.begin(), internalOutputs.end(),
              [](OutputComponent *a, OutputComponent *b) {
                  return a->getIndex() < b->getIndex();
              });
}

void CustomComponent::calculate() {
    if (!internalCircuit || !isValid()) return;

    for (size_t i = 0; i < inputs.size() && i < internalInputs.size(); ++i) {
        int value = inputs[i]->getValue();
        internalInputs[i]->setExternalValue(value);
    }

    int numComponents = static_cast<int>(internalCircuit->GetComponents().size());
    int passes = std::max(numComponents, 3);
    for (int p = 0; p < passes; ++p) {
        internalCircuit->update();
    }

    for (size_t i = 0; i < outputs.size() && i < internalOutputs.size(); ++i) {
        int value = internalOutputs[i]->getOutputValue();
        outputs[i]->setValue(value);
    }
}

std::string CustomComponent::GetLabel() {
    return std::format("{} : {}", GetId(), definitionName);
}

std::string CustomComponent::getType() const {
    return "CustomComponent";
}

void CustomComponent::draw(sf::RenderTarget &target) {
    sf::Vector2f pos = getPosition();

    body.setPosition(pos);
    target.draw(body);

    ResourceManager &rm = ResourceManager::getInstance();
    sf::Font &font = rm.getDefaultFont();
    sf::Text text(font);
    text.setString(definitionName);
    text.setCharacterSize(11);
    text.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = text.getLocalBounds();
    float textX = pos.x + (body.getSize().x - textBounds.size.x) / 2.f;
    float textY = pos.y + (body.getSize().y - textBounds.size.y) / 2.f - 5.f;
    text.setPosition(sf::Vector2f(textX, textY));
    target.draw(text);
}

void CustomComponent::drawLabel(sf::RenderTarget &target) {
    ResourceManager &rm = ResourceManager::getInstance();
    sf::Font &font = rm.getDefaultFont();
    sf::Text text(font);
    text.setString(GetLabel());
    text.setCharacterSize(14);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(position.x, position.y + body.getSize().y + 5.f));
    target.draw(text);
}

sf::FloatRect CustomComponent::getBounds() const {
    return sf::FloatRect(position, body.getSize());
}