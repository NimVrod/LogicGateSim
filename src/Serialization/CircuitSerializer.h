#pragma once
#include <string>

namespace tinyxml2 {
    class XMLElement;
}

class Circuit;

class CircuitSerializer {
public:
    static bool saveToFile(const Circuit &circuit, const std::string &filepath);

    static bool loadFromFile(Circuit &circuit, const std::string &filepath);

    static bool loadFromXmlString(Circuit &circuit, const std::string &xmlString);

    static std::string saveToXmlString(const Circuit &circuit);

    const static std::string currentVersion;

private:
    static bool loadFromXmlElement(Circuit &circuit, tinyxml2::XMLElement *root, bool strictVersion);
};