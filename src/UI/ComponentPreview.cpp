#include "ComponentPreview.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include <cmath>

// Include component headers for creating preview instances
#include "../Core/Components/Include/Button.h"
#include "../Core/Components/Include/AndGate.h"
#include "../Core/Components/Include/OrGate.h"
#include "../Core/Components/Include/NotGate.h"
#include "../Core/Components/Include/NandGate.h"
#include "../Core/Components/Include/NorGate.h"
#include "../Core/Components/Include/XorGate.h"
#include "../Core/Components/Include/XnorGate.h"

ComponentPreview::ComponentPreview() {
    // Initialize render texture
    if (renderTexture.resize({200, 120})) {
        textureInitialized = true;
        renderTexture.setSmooth(true);
    }
}

void ComponentPreview::setComponentType(const std::string& type, int numInputs) {
    if (type != currentType || numInputs != currentNumInputs) {
        currentType = type;
        currentNumInputs = numInputs;
        updatePreviewComponent();
        generateTruthTable(); // Generate after component is created
    }
}

void ComponentPreview::updatePreviewComponent() {
    previewComponent.reset();
    
    if (currentType.empty()) return;
    
    // Create component at origin first, then we'll center it
    sf::Vector2f origin(0.0f, 0.0f);
    
    // Create the actual component using existing classes
    if (currentType == "Button") {
        previewComponent = std::make_unique<Button>(0, origin);
    } else if (currentType == "AND") {
        previewComponent = std::make_unique<AndGate>(0, origin, currentNumInputs);
    } else if (currentType == "OR") {
        previewComponent = std::make_unique<OrGate>(0, origin, currentNumInputs);
    } else if (currentType == "NOT") {
        previewComponent = std::make_unique<NotGate>(0, origin);
    } else if (currentType == "NAND") {
        previewComponent = std::make_unique<NandGate>(0, origin, currentNumInputs);
    } else if (currentType == "NOR") {
        previewComponent = std::make_unique<NorGate>(0, origin, currentNumInputs);
    } else if (currentType == "XOR") {
        previewComponent = std::make_unique<XorGate>(0, origin, currentNumInputs);
    } else if (currentType == "XNOR") {
        previewComponent = std::make_unique<XnorGate>(0, origin, currentNumInputs);
    }
    
    if (previewComponent) {
        // Center the component in the render texture
        sf::FloatRect bounds = previewComponent->getBounds();
        float textureWidth = 200.0f;
        float textureHeight = 120.0f;
        
        float centerX = (textureWidth - bounds.size.x) / 2.0f;
        float centerY = (textureHeight - bounds.size.y) / 2.0f;
        
        previewComponent->setPosition(sf::Vector2f(centerX, centerY));
    }
    
    // Render to texture immediately
    renderComponentToTexture();
}

void ComponentPreview::renderComponentToTexture() {
    if (!textureInitialized || !previewComponent) return;
    
    renderTexture.clear(sf::Color(30, 30, 40));
    
    // Draw the component using its existing draw method
    previewComponent->draw(renderTexture);
    
    // Optionally draw pins
    if (drawPins) {
        previewComponent->drawPins(renderTexture);
    }
    
    renderTexture.display();
}

void ComponentPreview::renderUI() {
    if (!visible) return;
    
    ImGui::SetNextWindowSize(ImVec2(350, 450), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Component Preview", &visible)) {
        if (currentType.empty()) {
            ImGui::TextDisabled("Select a component to preview");
        } else {
            // Component info
            ImGui::Text("Component: %s", currentType.c_str());
            if (currentType != "Button" && currentType != "NOT") {
                ImGui::Text("Inputs: %d", currentNumInputs);
            }
            
            ImGui::Separator();
            
            // Visual preview using render texture
            ImGui::Text("Preview:");
            if (ImGui::Checkbox("Show Pins", &drawPins)) {
                renderComponentToTexture(); // Re-render when option changes
            }
            if (textureInitialized && previewComponent) {
                // Display the render texture as an ImGui image
                ImGui::Image(renderTexture, sf::Color::White, sf::Color(80, 80, 80));
            } else {
                ImGui::TextDisabled("Preview not available");
            }
            
            ImGui::Separator();
            
            // Truth table
            ImGui::Text("Truth Table:");
            renderTruthTableUI();
        }
    }
    ImGui::End();
}

void ComponentPreview::generateTruthTable() {
    headers.clear();
    rows.clear();
    
    if (currentType.empty()) return;
    
    if (currentType == "Button") {
        headers = {"State", "Output"};
        rows = {{"OFF", "0"}, {"ON", "1"}};
        return;
    }
    
    if (!previewComponent) return;
    
    int numInputs = static_cast<int>(previewComponent->getInputs().size());
    
    // Generate headers
    for (int i = 0; i < numInputs; i++) {
        headers.push_back(std::string(1, 'A' + i));
    }
    headers.push_back("OUT");
    
    // Generate all input permutations and use component's calculate()
    int numRows = 1 << numInputs;
    for (int r = 0; r < numRows; r++) {
        std::vector<std::string> row;
        
        // Set input values on the component
        const auto& inputs = previewComponent->getInputs();
        for (int i = 0; i < numInputs; i++) {
            // Bit order: MSB first (so input A is the leftmost bit)
            bool val = (r >> (numInputs - 1 - i)) & 1;
            row.push_back(val ? "1" : "0");
            inputs[i]->setValue(val ? 1 : 0);
        }
        
        // Use the component's own calculate method!
        previewComponent->calculate();
        
        // Get output from the component
        const auto& outputs = previewComponent->getOutputs();
        if (!outputs.empty()) {
            int outputVal = outputs[0]->getValue();
            row.push_back(outputVal >= 1 ? "1" : "0");
        } else {
            row.push_back("?");
        }
        
        rows.push_back(row);
    }

    // Reset the component's inputs and outputs to 0
    for (const auto& input : previewComponent->getInputs()) {
        input->setValue(0);
    }
    for (const auto& output : previewComponent->getOutputs()) {
        output->setValue(0);
    }
    previewComponent->calculate();
}

void ComponentPreview::renderTruthTableUI() {
    if (headers.empty()) return;
    
    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit;
    
    if (ImGui::BeginTable("TruthTable", static_cast<int>(headers.size()), flags)) {
        for (const auto& h : headers) {
            ImGui::TableSetupColumn(h.c_str(), ImGuiTableColumnFlags_WidthFixed, 30.0f);
        }
        ImGui::TableHeadersRow();
        
        for (const auto& row : rows) {
            ImGui::TableNextRow();
            for (size_t i = 0; i < row.size(); i++) {
                ImGui::TableSetColumnIndex(static_cast<int>(i));
                if (i == row.size() - 1 && row[i] == "1") {
                    ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.2f, 1.0f), "%s", row[i].c_str());
                } else {
                    ImGui::Text("%s", row[i].c_str());
                }
            }
        }
        ImGui::EndTable();
    }
}
