#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "../../Core/Component.h"
#include "ToggableImGuiWindow.h"

class ComponentPreview : public ToggableImGuiWindow {
public:
    ComponentPreview();
    ~ComponentPreview() override = default;
    
    void setComponentType(ComponentType type, int numInputs = 2);
    
    void renderUI() override;
    
    ComponentType getCurrentComponentType() const { return currentComponentType; }
    int getCurrentNumInputs() const { return currentNumInputs; }
    
private:
    ComponentType currentComponentType = ComponentType::UNDEFINED;
    std::string customTypeName;
    int currentNumInputs = 2;
    
    sf::RenderTexture renderTexture;
    bool textureInitialized = false;
    bool drawPins = true;
    
    std::unique_ptr<Component> previewComponent;
    
    std::vector<std::string> headers;
    std::vector<std::vector<std::string>> rows;
    
    void generateTruthTable();
    void renderTruthTableUI();
    void updatePreviewComponent();
    void renderComponentToTexture();

    static std::string componentTypeToString(ComponentType type);
};
