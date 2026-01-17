#include "CustomComponentManager.h"
#include <tinyxml2.h>
#include <filesystem>

#include "CustomComponentDefinition.h"

CustomComponentManager &CustomComponentManager::getInstance() {
    static CustomComponentManager instance;
    return instance;
}

bool CustomComponentManager::addDefinition(const CustomComponentDefinition &definition) {
    if (definition.name.empty()) {
        return false;
    }

    if (definitions.find(definition.name) != definitions.end()) {
        return false; // Already exists
    }

    definitions[definition.name] = definition;
    return true;
}

const CustomComponentDefinition *CustomComponentManager::getDefinition(const std::string &name) const {
    auto it = definitions.find(name);
    if (it != definitions.end()) {
        return &it->second;
    }
    return nullptr;
}

std::vector<std::string> CustomComponentManager::getDefinitionNames() const {
    std::vector<std::string> names;
    names.reserve(definitions.size());
    for (const auto &pair: definitions) {
        names.push_back(pair.first);
    }
    return names;
}

bool CustomComponentManager::hasDefinition(const std::string &name) const {
    return definitions.find(name) != definitions.end();
}

bool CustomComponentManager::removeDefinition(const std::string &name) {
    return definitions.erase(name) > 0;
}

void CustomComponentManager::clear() {
    definitions.clear();
}

bool CustomComponentManager::saveToFile(const std::string &filepath) const {
    tinyxml2::XMLDocument doc;

    auto *declaration = doc.NewDeclaration();
    doc.InsertFirstChild(declaration);

    auto *root = doc.NewElement("CustomComponents");
    root->SetAttribute("version", "1.0");
    doc.InsertEndChild(root);

    for (const auto &pair: definitions) {
        const auto &def = pair.second;

        auto *compElem = doc.NewElement("CustomComponent");
        compElem->SetAttribute("name", def.name.c_str());
        compElem->SetAttribute("numInputs", def.numInputs);
        compElem->SetAttribute("numOutputs", def.numOutputs);

        // Store circuit XML as CDATA to preserve formatting
        auto *circuitElem = doc.NewElement("Circuit");
        auto *cdata = doc.NewText(def.circuitXml.c_str());
        cdata->SetCData(true);
        circuitElem->InsertEndChild(cdata);
        compElem->InsertEndChild(circuitElem);

        root->InsertEndChild(compElem);
    }

    return doc.SaveFile(filepath.c_str()) == tinyxml2::XML_SUCCESS;
}

bool CustomComponentManager::loadFromFile(const std::string &filepath) {
    if (!std::filesystem::exists(filepath)) {
        return false;
    }

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filepath.c_str()) != tinyxml2::XML_SUCCESS) {
        return false;
    }

    auto *root = doc.FirstChildElement("CustomComponents");
    if (!root) {
        return false;
    }

    for (auto *compElem = root->FirstChildElement("CustomComponent");
         compElem;
         compElem = compElem->NextSiblingElement("CustomComponent")) {
        CustomComponentDefinition def;

        const char *name = compElem->Attribute("name");
        if (name)
            def.name = name;
        else
            continue; // Skip entries without a name

        def.numInputs = compElem->IntAttribute("numInputs", 0);
        def.numOutputs = compElem->IntAttribute("numOutputs", 0);

        auto *circuitElem = compElem->FirstChildElement("Circuit");
        if (circuitElem && circuitElem->GetText()) {
            def.circuitXml = circuitElem->GetText();
        }

        definitions[def.name] = def;
    }

    return true;
}