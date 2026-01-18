#include "CircuitSerializer.h"
#include "../Simulation/Circuit.h"
#include "../Core/Components/Include/Button.h"
#include "../Core/Components/Include/InputComponent.h"
#include "../Core/Components/Include/OutputComponent.h"
#include "../Core/Components/CustomComponent/CustomComponent.h"
#include <tinyxml2.h>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include "Core/Components/Include/ComponentFactory.h"

const std::string CircuitSerializer::currentVersion = "0.2";


std::string CircuitSerializer::saveToXmlString(const Circuit &circuit) {
    tinyxml2::XMLDocument doc;

    auto *declaration = doc.NewDeclaration();
    doc.InsertFirstChild(declaration);

    auto *root = doc.NewElement("Circuit");
    root->SetAttribute("version", currentVersion.c_str());
    doc.InsertEndChild(root);

    auto *componentsElem = doc.NewElement("Components");
    root->InsertEndChild(componentsElem);

    for (const auto &comp: circuit.GetComponents()) {
        auto *compElem = doc.NewElement("Component");
        compElem->SetAttribute("id", comp->GetId());
        compElem->SetAttribute("type", comp->getType().c_str());
        compElem->SetAttribute("x", comp->getPosition().x);
        compElem->SetAttribute("y", comp->getPosition().y);
        compElem->SetAttribute("numInputs", static_cast<int>(comp->getInputs().size()));
        compElem->SetAttribute("numOutputs", static_cast<int>(comp->getOutputs().size()));

        if (comp->getType() == "Input Pin") {
            if (auto *inputComp = dynamic_cast<InputComponent *>(comp.get()))
                compElem->SetAttribute("index", inputComp->getIndex());
        } else if (comp->getType() == "Output Pin") {
            if (auto *outputComp = dynamic_cast<OutputComponent *>(comp.get()))
                compElem->SetAttribute("index", outputComp->getIndex());
        } else if (comp->getType() == "CustomComponent") {
            if (auto *customComp = dynamic_cast<CustomComponent *>(comp.get())) {
                compElem->SetAttribute("customType", customComp->getDefinitionName().c_str());
            }
        }

        componentsElem->InsertEndChild(compElem);
    }

    auto *wiresElem = doc.NewElement("Wires");
    root->InsertEndChild(wiresElem);

    for (const auto &wire: circuit.getWires()) {
        auto *wireElem = doc.NewElement("Wire");

        Pin *startPin = wire->getStartPin();
        Pin *endPin = wire->getEndPin();

        int fromCompId = startPin->getParent()->GetId();
        int toCompId = endPin->getParent()->GetId();

        int fromPinIdx = 0;
        const auto &outputs = startPin->getParent()->getOutputs();
        for (size_t i = 0; i < outputs.size(); ++i) {
            if (outputs[i].get() == startPin) {
                fromPinIdx = static_cast<int>(i);
                break;
            }
        }

        int toPinIdx = 0;
        const auto &inputs = endPin->getParent()->getInputs();
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

bool CircuitSerializer::saveToFile(const Circuit &circuit, const std::string &filepath) {
    std::string xmlContent = saveToXmlString(circuit);
    std::ofstream file(filepath);
    if (!file.is_open())
        return false;
    file << xmlContent;
    return file.good();
}

bool CircuitSerializer::loadFromXmlElement(Circuit &circuit, tinyxml2::XMLElement *root, bool strictVersion) {
    const char *version = root->Attribute("version");
    if (strictVersion) {
        if (!version || strcmp(version, currentVersion.c_str()) != 0) {
            std::cout << "Warning: Circuit version mismatch. Expected " << currentVersion << " but found " << (
                version ? version : "unknown") << ".\n";
            return false;
        }
    } else if (version && strcmp(version, currentVersion.c_str()) != 0) {
        std::cout << "Warning: Circuit version mismatch in XML string.\n";
    }

    circuit.clear();

    std::unordered_map<int, Component *> idToComponent;
    int maxId = 0;

    auto *componentsElem = root->FirstChildElement("Components");
    if (componentsElem) {
        for (auto *compElem = componentsElem->FirstChildElement("Component");
             compElem;
             compElem = compElem->NextSiblingElement("Component")) {
            const char *type = compElem->Attribute("type");
            if (!type) continue;

            int id = compElem->IntAttribute("id");
            // For Input Pin and Output Pin, use the index attribute; otherwise use numInputs
            int thirdParam = compElem->IntAttribute("numInputs", 2);
            if (strcmp(type, "Input Pin") == 0 || strcmp(type, "Output Pin") == 0) {
                thirdParam = compElem->IntAttribute("index", 0);
            }
            if (strcmp(type, "CustomComponent") == 0) {
                const char *customType = compElem->Attribute("customType");
                if (customType)
                    type = customType; //Component factory create handles custom component types
            }

            auto comp = ComponentFactory::create(type,
                                                 id,
                                                 sf::Vector2f(compElem->FloatAttribute("x"),
                                                              compElem->FloatAttribute("y")),
                                                 thirdParam);
            if (comp) {
                idToComponent[id] = comp.get();
                maxId = std::max(maxId, id);
                circuit.components.push_back(std::move(comp));
            }
        }
    }

    circuit.setNextId(maxId + 1);

    auto *wiresElem = root->FirstChildElement("Wires");
    if (wiresElem) {
        for (auto *wireElem = wiresElem->FirstChildElement("Wire");
             wireElem;
             wireElem = wireElem->NextSiblingElement("Wire")) {
            int fromCompId = wireElem->IntAttribute("fromComponent");
            int fromPinIdx = wireElem->IntAttribute("fromPin");
            int toCompId = wireElem->IntAttribute("toComponent");
            int toPinIdx = wireElem->IntAttribute("toPin");

            auto fromIt = idToComponent.find(fromCompId);
            auto toIt = idToComponent.find(toCompId);

            if (fromIt != idToComponent.end() && toIt != idToComponent.end()) {
                Component *fromComp = fromIt->second;
                Component *toComp = toIt->second;

                const auto &outputs = fromComp->getOutputs();
                const auto &inputs = toComp->getInputs();

                if (fromPinIdx < static_cast<int>(outputs.size()) &&
                    toPinIdx < static_cast<int>(inputs.size())) {
                    Pin *startPin = outputs[fromPinIdx].get();
                    Pin *endPin = inputs[toPinIdx].get();
                    circuit.addWire(startPin, endPin);
                }
            }
        }
    }

    return true;
}

bool CircuitSerializer::loadFromFile(Circuit &circuit, const std::string &filepath) {
    if (!std::filesystem::exists(filepath))
        return false;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filepath.c_str()) != tinyxml2::XML_SUCCESS)
        return false;

    auto *root = doc.FirstChildElement("Circuit");
    if (!root)
        return false;

    return loadFromXmlElement(circuit, root, true);
}

bool CircuitSerializer::loadFromXmlString(Circuit &circuit, const std::string &xmlString) {
    if (xmlString.empty())
        return false;

    tinyxml2::XMLDocument doc;
    if (doc.Parse(xmlString.c_str()) != tinyxml2::XML_SUCCESS)
        return false;

    auto *root = doc.FirstChildElement("Circuit");
    if (!root)
        return false;

    return loadFromXmlElement(circuit, root, false);
}