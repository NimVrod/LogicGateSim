#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <memory>
#include <string>
#include <vector>
#include "../../Component.h"

// Forward declaration
class Circuit;
class InputComponent;
class OutputComponent;
struct CustomComponentDefinition;

// CustomComponent is a component that contains an internal sub-circuit.
// It acts as a black box that can be reused, with its inputs and outputs
// mapped to InputComponent and OutputComponent inside the sub-circuit.
class CustomComponent : public Component {
public:
    // Create from a definition
    CustomComponent(int id, sf::Vector2f position, const std::string& definitionName);
    
    ~CustomComponent();
    
    void calculate() override;
    std::string GetLabel() override;
    std::string getType() const override;
    ComponentType GetType() override { return ComponentType::CUSTOM; }
    void draw(sf::RenderTarget& target) override;
    void drawLabel(sf::RenderTarget& target) override;
    sf::FloatRect getBounds() const override;
    
    // Get the definition name for serialization
    const std::string& getDefinitionName() const { return definitionName; }
    
    // Check if the internal circuit was loaded successfully
    bool isValid() const { return internalCircuit != nullptr; }

protected:
    sf::RectangleShape body;
    
private:
    std::string definitionName;
    std::unique_ptr<Circuit> internalCircuit;
    
    // Cached pointers to internal Input/Output components (sorted by index)
    std::vector<InputComponent*> internalInputs;
    std::vector<OutputComponent*> internalOutputs;
    
    // Initialize the internal circuit from the definition
    void initFromDefinition(const CustomComponentDefinition* def);
    
    // Find and cache internal Input/Output components
    void cacheInternalComponents();
};
