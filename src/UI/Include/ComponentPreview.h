#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "../../Core/Components/Include/Component.h"
#include "ToggableImGuiWindow.h"

class ComponentPreview : public ToggableImGuiWindow {
public:
    ComponentPreview();
    ~ComponentPreview() override = default;
    
    void setComponentType(const std::string& typeName, int numInputs = 2);
    
    void renderUI() override;
    
    const std::string& getCurrentTypeName() const { return currentTypeName; }
    int getCurrentNumInputs() const { return currentNumInputs; }
    
private:
    std::string currentTypeName;
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
};
