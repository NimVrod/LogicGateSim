#pragma once
#include <string>

class Circuit;

class CircuitSerializer {
private:


public:
    static bool saveToFile(const Circuit& circuit, const std::string& filepath);
    static bool loadFromFile(Circuit& circuit, const std::string& filepath);
    const static std::string currentVersion;
};
