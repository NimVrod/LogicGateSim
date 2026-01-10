#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "../Core/Component.h"

class ComponentPreview {
public:
    ComponentPreview();
    ~ComponentPreview() = default;
    
    // Set the component type to preview
    void setComponentType(const std::string& type, int numInputs = 2);
    
    // Render the ImGui window (call during main window UI rendering)
    void renderUI();
    
    // Get current selection info
    const std::string& getCurrentType() const { return currentType; }
    int getCurrentNumInputs() const { return currentNumInputs; }
    
    // Control visibility
    void show() { visible = true; }
    void hide() { visible = false; }
    bool isVisible() const { return visible; }
    void toggle() { visible = !visible; }
    
private:
    std::string currentType;
    int currentNumInputs = 2;
    bool visible = false;
    
    // RenderTexture for component preview
    sf::RenderTexture renderTexture;
    bool textureInitialized = false;
    bool drawPins = true;  // Option to show pins in preview
    
    // The actual component to preview
    std::unique_ptr<Component> previewComponent;
    
    // Truth table data
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;
    
    void generateTruthTable();
    void renderTruthTableUI();
    void updatePreviewComponent();
    void renderComponentToTexture();
};
