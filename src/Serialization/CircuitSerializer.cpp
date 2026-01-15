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
#include "../Core/Components/Include/InputComponent.h"
#include "../Core/Components/Include/OutputComponent.h"
#include "../Core/Components/Include/CustomComponent.h"
#include "../Core/Components/Include/ClockComponent.h"
#include "../Core/Components/Include/LEDComponent.h"
#include "../Core/Components/Include/SRFlipFlop.h"
#include "../Core/Components/Include/DFlipFlop.h"
#include "../Core/Components/Include/JKFlipFlop.h"
#include "../Core/Components/Include/TFlipFlop.h"
#include <tinyxml2.h>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <functional>

const std::string CircuitSerializer::currentVersion = "0.2";

namespace {
    struct ComponentParams {
        int id;
        sf::Vector2f position;
        int numInputs;
        tinyxml2::XMLElement* element;
    };

    using ComponentFactory = std::function<std::unique_ptr<Component>(const ComponentParams&)>;

    // No switch case for strings in C++, so we use a map of lambda factories
    const std::unordered_map<std::string, ComponentFactory>& getComponentFactories() {
        static const std::unordered_map<std::string, ComponentFactory> factories = {
            {"Button", [](const ComponentParams& p) { return std::make_unique<Button>(p.id, p.position); }},
            {"AndGate", [](const ComponentParams& p) { return std::make_unique<AndGate>(p.id, p.position, p.numInputs); }},
            {"OrGate", [](const ComponentParams& p) { return std::make_unique<OrGate>(p.id, p.position, p.numInputs); }},
            {"NotGate", [](const ComponentParams& p) { return std::make_unique<NotGate>(p.id, p.position); }},
            {"NandGate", [](const ComponentParams& p) { return std::make_unique<NandGate>(p.id, p.position, p.numInputs); }},
            {"NorGate", [](const ComponentParams& p) { return std::make_unique<NorGate>(p.id, p.position, p.numInputs); }},
            {"XorGate", [](const ComponentParams& p) { return std::make_unique<XorGate>(p.id, p.position, p.numInputs); }},
            {"XnorGate", [](const ComponentParams& p) { return std::make_unique<XnorGate>(p.id, p.position, p.numInputs); }},
            {"InputComponent", [](const ComponentParams& p) {
                int index = p.element->IntAttribute("index", 0);
                return std::make_unique<InputComponent>(p.id, p.position, index);
            }},
            {"OutputComponent", [](const ComponentParams& p) {
                int index = p.element->IntAttribute("index", 0);
                return std::make_unique<OutputComponent>(p.id, p.position, index);
            }},
            {"CustomComponent", [](const ComponentParams& p) -> std::unique_ptr<Component> {
                const char* defName = p.element->Attribute("definitionName");
                if (defName)
                    return std::make_unique<CustomComponent>(p.id, p.position, defName);
                return nullptr;
            }},
            {"ClockComponent", [](const ComponentParams& p) { return std::make_unique<ClockComponent>(p.id, p.position); }},
            {"LEDComponent", [](const ComponentParams& p) { return std::make_unique<LEDComponent>(p.id, p.position); }},
            {"SRFlipFlop", [](const ComponentParams& p) { return std::make_unique<SRFlipFlop>(p.id, p.position); }},
            {"DFlipFlop", [](const ComponentParams& p) { return std::make_unique<DFlipFlop>(p.id, p.position); }},
            {"JKFlipFlop", [](const ComponentParams& p) { return std::make_unique<JKFlipFlop>(p.id, p.position); }},
            {"TFlipFlop", [](const ComponentParams& p) { return std::make_unique<TFlipFlop>(p.id, p.position); }},
        };
        return factories;
    }
}

std::string CircuitSerializer::saveToXmlString(const Circuit& circuit) {
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
        
        if (comp->GetType() == ComponentType::INPUT) {
            if (auto* inputComp = dynamic_cast<InputComponent*>(comp.get()))
                compElem->SetAttribute("index", inputComp->getIndex());
        } else if (comp->GetType() == ComponentType::OUTPUT) {
            if (auto* outputComp = dynamic_cast<OutputComponent*>(comp.get()))
                compElem->SetAttribute("index", outputComp->getIndex());
        } else if (comp->GetType() == ComponentType::CUSTOM) {
            if (auto* customComp = dynamic_cast<CustomComponent*>(comp.get()))
                compElem->SetAttribute("definitionName", customComp->getDefinitionName().c_str());
        }

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

    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);
    return std::string(printer.CStr());
}

bool CircuitSerializer::saveToFile(const Circuit& circuit, const std::string& filepath) {
    std::string xmlContent = saveToXmlString(circuit);
    std::ofstream file(filepath);
    if (!file.is_open())
        return false;
    file << xmlContent;
    return file.good();
}

bool CircuitSerializer::loadFromXmlElement(Circuit& circuit, tinyxml2::XMLElement* root, bool strictVersion) {
    const char* version = root->Attribute("version");
    if (strictVersion) {
        if (!version || strcmp(version, currentVersion.c_str()) != 0) {
            std::cout << "Warning: Circuit version mismatch. Expected " << currentVersion<< " but found " << (version ? version : "unknown") << ".\n";
            return false;
        }
    } else if (version && strcmp(version, currentVersion.c_str()) != 0) {
        std::cout << "Warning: Circuit version mismatch in XML string.\n";
    }

    circuit.clear();

    std::unordered_map<int, Component*> idToComponent;
    int maxId = 0;
    const auto& factories = getComponentFactories();

    auto* componentsElem = root->FirstChildElement("Components");
    if (componentsElem) {
        for (auto* compElem = componentsElem->FirstChildElement("Component");
             compElem;
             compElem = compElem->NextSiblingElement("Component")) {

            const char* type = compElem->Attribute("type");
            if (!type) continue;

            ComponentParams params{
                compElem->IntAttribute("id"),
                sf::Vector2f(compElem->FloatAttribute("x"), compElem->FloatAttribute("y")),
                compElem->IntAttribute("numInputs", 2),
                compElem
            };

            auto it = factories.find(type);
            if (it != factories.end()) {
                auto comp = it->second(params);
                if (comp) {
                    idToComponent[params.id] = comp.get();
                    maxId = std::max(maxId, params.id);
                    circuit.components.push_back(std::move(comp));
                }
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

bool CircuitSerializer::loadFromFile(Circuit& circuit, const std::string& filepath) {
    if (!std::filesystem::exists(filepath))
        return false;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filepath.c_str()) != tinyxml2::XML_SUCCESS)
        return false;

    auto* root = doc.FirstChildElement("Circuit");
    if (!root)
        return false;

    return loadFromXmlElement(circuit, root, true);
}

bool CircuitSerializer::loadFromXmlString(Circuit& circuit, const std::string& xmlString) {
    if (xmlString.empty())
        return false;

    tinyxml2::XMLDocument doc;
    if (doc.Parse(xmlString.c_str()) != tinyxml2::XML_SUCCESS)
        return false;

    auto* root = doc.FirstChildElement("Circuit");
    if (!root)
        return false;

    return loadFromXmlElement(circuit, root, false);
}
