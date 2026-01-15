#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include "CustomComponentDefinition.h"

// CustomComponentManager is a singleton that stores all custom component definitions.
// It handles loading/saving definitions from/to a file.
class CustomComponentManager {
public:
    static CustomComponentManager& getInstance();
    
    // Prevent copying
    CustomComponentManager(const CustomComponentManager&) = delete;
    CustomComponentManager& operator=(const CustomComponentManager&) = delete;
    
    // Add a new definition (returns true if successful, false if name already exists)
    bool addDefinition(const CustomComponentDefinition& definition);
    
    // Get a definition by name (returns nullptr if not found)
    const CustomComponentDefinition* getDefinition(const std::string& name) const;
    
    // Get names of all available definitions
    std::vector<std::string> getDefinitionNames() const;
    
    // Check if a definition exists
    bool hasDefinition(const std::string& name) const;
    
    // Remove a definition
    bool removeDefinition(const std::string& name);
    
    // Save all definitions to file
    bool saveToFile(const std::string& filepath) const;
    
    // Load definitions from file
    bool loadFromFile(const std::string& filepath);
    
    // Clear all definitions
    void clear();
    
    // Get number of definitions
    size_t count() const { return definitions.size(); }

private:
    CustomComponentManager() = default;
    
    std::unordered_map<std::string, CustomComponentDefinition> definitions;
};
