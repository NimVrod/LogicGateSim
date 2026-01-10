#include "CircuitSerializer.h"
#include "../Simulation/Circuit.h"
#include "../Core/Components/Include/Button.h"
#include "../Core/Components/Include/AndGate.h"
#include "../Core/Components/Include/OrGate.h"
#include "../Core/Components/Include/NotGate.h"
#include "../Core/Components/Include/NandGate.h"
#include "../Core/Components/Include/NorGate.h"
#include "../Core/Components/Include/XorGate.h"
#include "../Core/Components/Include/XnorGate.h"
#include <tinyxml2.h>
#include <filesystem>
#include <iostream>
#include <unordered_map>

const std::string CircuitSerializer::currentVersion = "0.1";

bool CircuitSerializer::saveToFile(const Circuit& circuit, const std::string& filepath) {
    tinyxml2::XMLDocument doc;

    auto* declaration = doc.NewDeclaration();
    doc.InsertFirstChild(declaration);

    auto* root = doc.NewElement("Circuit");
    root->SetAttribute("version", currentVersion.c_str());
    doc.InsertEndChild(root);

    auto* componentsElem = doc.NewElement("Components");
    root->InsertEndChild(componentsElem);

    for (const auto& comp : circuit.GetComponents()) {
        auto* compElem = doc.NewElement("Component");
        compElem->SetAttribute("id", comp->GetId());
        compElem->SetAttribute("type", comp->getType().c_str());
        compElem->SetAttribute("x", comp->getPosition().x);
        compElem->SetAttribute("y", comp->getPosition().y);
        compElem->SetAttribute("numInputs", static_cast<int>(comp->getInputs().size()));
        compElem->SetAttribute("numOutputs", static_cast<int>(comp->getOutputs().size()));

        componentsElem->InsertEndChild(compElem);
    }

    auto* wiresElem = doc.NewElement("Wires");
    root->InsertEndChild(wiresElem);

    for (const auto& wire : circuit.getWires()) {
        auto* wireElem = doc.NewElement("Wire");

        Pin* startPin = wire->getStartPin();
        Pin* endPin = wire->getEndPin();

        int fromCompId = startPin->getParent()->GetId();
        int toCompId = endPin->getParent()->GetId();

        int fromPinIdx = 0;
        const auto& outputs = startPin->getParent()->getOutputs();
        for (size_t i = 0; i < outputs.size(); ++i) {
            if (outputs[i].get() == startPin) {
                fromPinIdx = static_cast<int>(i);
                break;
            }
        }

        int toPinIdx = 0;
        const auto& inputs = endPin->getParent()->getInputs();
        for (size_t i = 0; i < inputs.size(); ++i) {
            if (inputs[i].get() == endPin) {
                toPinIdx = static_cast<int>(i);
                break;
            }
        }

        wireElem->SetAttribute("fromComponent", fromCompId);
        wireElem->SetAttribute("fromPin", fromPinIdx);
        wireElem->SetAttribute("toComponent", toCompId);
        wireElem->SetAttribute("toPin", toPinIdx);

        wiresElem->InsertEndChild(wireElem);
    }

    return doc.SaveFile(filepath.c_str()) == tinyxml2::XML_SUCCESS;
}

bool CircuitSerializer::loadFromFile(Circuit& circuit, const std::string& filepath) {
    if (!std::filesystem::exists(filepath)) {
        return false;
    }

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filepath.c_str()) != tinyxml2::XML_SUCCESS) {
        return false;
    }

    auto* root = doc.FirstChildElement("Circuit");
    if (!root) {
        return false;
    }

    if (strcmp(root->Attribute("version"), currentVersion.c_str()) != 0) {

        std::cout << "Warning: Circuit version mismatch. Expected " << currentVersion
                  << " but found " << (root->Attribute("version") ? root->Attribute("version") : "unknown") << ".\n";
        return false;
    }

    circuit.clear();

    std::unordered_map<int, Component*> idToComponent;
    int maxId = 0;

    auto* componentsElem = root->FirstChildElement("Components");
    if (componentsElem) {
        for (auto* compElem = componentsElem->FirstChildElement("Component");
             compElem;
             compElem = compElem->NextSiblingElement("Component")) {

            int id = compElem->IntAttribute("id");
            const char* type = compElem->Attribute("type");
            float x = compElem->FloatAttribute("x");
            float y = compElem->FloatAttribute("y");
            int numInputs = compElem->IntAttribute("numInputs", 2);
            int numOutputs = compElem->IntAttribute("numOutputs", 1);

            sf::Vector2f pos(x, y);
            std::unique_ptr<Component> comp;

            if (strcmp(type, "Button") == 0) {
                comp = std::make_unique<Button>(id, pos);
            } else if (strcmp(type, "AndGate") == 0) {
                comp = std::make_unique<AndGate>(id, pos, numInputs);
            } else if (strcmp(type, "OrGate") == 0) {
                comp = std::make_unique<OrGate>(id, pos, numInputs);
            } else if (strcmp(type, "NotGate") == 0) {
                comp = std::make_unique<NotGate>(id, pos);
            } else if (strcmp(type, "NandGate") == 0) {
                comp = std::make_unique<NandGate>(id, pos, numInputs);
            } else if (strcmp(type, "NorGate") == 0) {
                comp = std::make_unique<NorGate>(id, pos, numInputs);
            } else if (strcmp(type, "XorGate") == 0) {
                comp = std::make_unique<XorGate>(id, pos, numInputs);
            } else if (strcmp(type, "XnorGate") == 0) {
                comp = std::make_unique<XnorGate>(id, pos, numInputs);
            }

            if (comp) {
                idToComponent[id] = comp.get();
                maxId = std::max(maxId, id);
                circuit.components.push_back(std::move(comp));
            }
        }
    }

    circuit.setNextId(maxId + 1);

    auto* wiresElem = root->FirstChildElement("Wires");
    if (wiresElem) {
        for (auto* wireElem = wiresElem->FirstChildElement("Wire");
             wireElem;
             wireElem = wireElem->NextSiblingElement("Wire")) {

            int fromCompId = wireElem->IntAttribute("fromComponent");
            int fromPinIdx = wireElem->IntAttribute("fromPin");
            int toCompId = wireElem->IntAttribute("toComponent");
            int toPinIdx = wireElem->IntAttribute("toPin");

            auto fromIt = idToComponent.find(fromCompId);
            auto toIt = idToComponent.find(toCompId);

            if (fromIt != idToComponent.end() && toIt != idToComponent.end()) {
                Component* fromComp = fromIt->second;
                Component* toComp = toIt->second;

                const auto& outputs = fromComp->getOutputs();
                const auto& inputs = toComp->getInputs();

                if (fromPinIdx < static_cast<int>(outputs.size()) &&
                    toPinIdx < static_cast<int>(inputs.size())) {
                    Pin* startPin = outputs[fromPinIdx].get();
                    Pin* endPin = inputs[toPinIdx].get();
                    circuit.addWire(startPin, endPin);
                }
            }
        }
    }

    return true;
}
