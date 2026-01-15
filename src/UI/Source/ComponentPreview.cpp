#include "../Include/ComponentPreview.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include <cmath>

#include "../../Core/Components/Include/Button.h"
#include "../../Core/Components/Include/AndGate.h"
#include "../../Core/Components/Include/OrGate.h"
#include "../../Core/Components/Include/NotGate.h"
#include "../../Core/Components/Include/NandGate.h"
#include "../../Core/Components/Include/NorGate.h"
#include "../../Core/Components/Include/XorGate.h"
#include "../../Core/Components/Include/XnorGate.h"
#include "../../Core/Components/Include/InputComponent.h"
#include "../../Core/Components/Include/OutputComponent.h"
#include "../../Core/Components/Include/CustomComponent.h"
#include "../../Core/Components/Include/ClockComponent.h"
#include "../../Core/Components/Include/LEDComponent.h"
#include "../../Core/Components/Include/SRFlipFlop.h"
#include "../../Core/Components/Include/DFlipFlop.h"
#include "../../Core/Components/Include/JKFlipFlop.h"
#include "../../Core/Components/Include/TFlipFlop.h"
#include "../../Core/Components/CustomComponent/CustomComponentManager.h"

std::string ComponentPreview::componentTypeToString(const ComponentType type) {
    switch (type) {
        case ComponentType::AND: return "AND";
        case ComponentType::OR: return "OR";
        case ComponentType::NOT: return "NOT";
        case ComponentType::NAND: return "NAND";
        case ComponentType::NOR: return "NOR";
        case ComponentType::XOR: return "XOR";
        case ComponentType::XNOR: return "XNOR";
        case ComponentType::BUTTON: return "Button";
        case ComponentType::LED: return "LED";
        case ComponentType::CLOCK: return "Clock";
        case ComponentType::INPUT: return "Input Pin";
        case ComponentType::OUTPUT: return "Output Pin";
        case ComponentType::SR_FF: return "SR FF";
        case ComponentType::D_FF: return "D FF";
        case ComponentType::JK_FF: return "JK FF";
        case ComponentType::T_FF: return "T FF";
        case ComponentType::CUSTOM: return "Custom";
        case ComponentType::UNDEFINED: return "";
    }
    return "";
}


ComponentPreview::ComponentPreview() {
    if (renderTexture.resize({200, 120})) {
        textureInitialized = true;
        renderTexture.setSmooth(true);
    }
}

void ComponentPreview::setComponentType(ComponentType type, int numInputs) {
    if (type != currentComponentType || numInputs != currentNumInputs) {
        currentComponentType = type;
        currentNumInputs = numInputs;
        updatePreviewComponent();
        generateTruthTable();
    }
}

void ComponentPreview::updatePreviewComponent() {
    previewComponent.reset();
    
    if (currentComponentType == ComponentType::UNDEFINED) return;

    sf::Vector2f origin(0.0f, 0.0f);
    
    switch (currentComponentType) {
        case ComponentType::BUTTON:
            previewComponent = std::make_unique<Button>(0, origin);
            break;
        case ComponentType::AND:
            previewComponent = std::make_unique<AndGate>(0, origin, currentNumInputs);
            break;
        case ComponentType::OR:
            previewComponent = std::make_unique<OrGate>(0, origin, currentNumInputs);
            break;
        case ComponentType::NOT:
            previewComponent = std::make_unique<NotGate>(0, origin);
            break;
        case ComponentType::NAND:
            previewComponent = std::make_unique<NandGate>(0, origin, currentNumInputs);
            break;
        case ComponentType::NOR:
            previewComponent = std::make_unique<NorGate>(0, origin, currentNumInputs);
            break;
        case ComponentType::XOR:
            previewComponent = std::make_unique<XorGate>(0, origin, currentNumInputs);
            break;
        case ComponentType::XNOR:
            previewComponent = std::make_unique<XnorGate>(0, origin, currentNumInputs);
            break;
        case ComponentType::INPUT:
            previewComponent = std::make_unique<InputComponent>(0, origin, 0);
            break;
        case ComponentType::OUTPUT:
            previewComponent = std::make_unique<OutputComponent>(0, origin, 0);
            break;
        case ComponentType::CLOCK:
            previewComponent = std::make_unique<ClockComponent>(0, origin);
            break;
        case ComponentType::LED:
            previewComponent = std::make_unique<LEDComponent>(0, origin);
            break;
        case ComponentType::SR_FF:
            previewComponent = std::make_unique<SRFlipFlop>(0, origin);
            break;
        case ComponentType::D_FF:
            previewComponent = std::make_unique<DFlipFlop>(0, origin);
            break;
        case ComponentType::JK_FF:
            previewComponent = std::make_unique<JKFlipFlop>(0, origin);
            break;
        case ComponentType::T_FF:
            previewComponent = std::make_unique<TFlipFlop>(0, origin);
            break;
        case ComponentType::CUSTOM:
            if (CustomComponentManager::getInstance().hasDefinition(customTypeName)) {
                previewComponent = std::make_unique<CustomComponent>(0, origin, customTypeName);
            }
            break;
        case ComponentType::UNDEFINED:
            break;
    }
    
    if (previewComponent) {
        sf::FloatRect bounds = previewComponent->getBounds();
        float textureWidth = 200.0f;
        float textureHeight = 120.0f;
        
        float centerX = (textureWidth - bounds.size.x) / 2.0f;
        float centerY = (textureHeight - bounds.size.y) / 2.0f;
        
        previewComponent->setPosition(sf::Vector2f(centerX, centerY));
    }
    
    renderComponentToTexture();
}

void ComponentPreview::renderComponentToTexture() {
    if (!textureInitialized || !previewComponent) return;

    renderTexture.clear(sf::Color(30, 30, 40));
    previewComponent->draw(renderTexture);

    if (drawPins) {
        previewComponent->drawPins(renderTexture);
    }
    
    renderTexture.display();
}

void ComponentPreview::renderUI() {
    if (!visible) return;
    
    ImGui::SetNextWindowSize(ImVec2(350, 450), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("Component Preview", &visible)) {
        if (currentComponentType == ComponentType::UNDEFINED) {
            ImGui::TextDisabled("Select a component to preview");
        } else {
            std::string typeStr = componentTypeToString(currentComponentType);
            ImGui::Text("Component: %s", typeStr.c_str());
            if (currentComponentType != ComponentType::BUTTON && 
                currentComponentType != ComponentType::NOT && 
                previewComponent) {
                ImGui::Text("Inputs: %zu", previewComponent->getInputs().size());
            }
            
            ImGui::Separator();

            ImGui::Text("Preview:");
            if (ImGui::Checkbox("Show Pins", &drawPins)) {
                renderComponentToTexture();
            }
            if (textureInitialized && previewComponent) {
                ImGui::Image(renderTexture, sf::Color::White, sf::Color(80, 80, 80));
            } else {
                ImGui::TextDisabled("Preview not available");
            }
            
            ImGui::Separator();

            ImGui::Text("Truth Table:");
            renderTruthTableUI();
        }
    }
    ImGui::End();
}

void ComponentPreview::generateTruthTable() {
    headers.clear();
    rows.clear();
    
    if (currentComponentType == ComponentType::UNDEFINED) return;
    
    switch (currentComponentType) {
        case ComponentType::BUTTON:
            headers = {"State", "Output"};
            rows = {{"OFF", "0"}, {"ON", "1"}};
            return;
        case ComponentType::INPUT:
            headers = {"Set Value", "Output"};
            rows = {{"0", "0"}, {"1", "1"}};
            return;
        case ComponentType::OUTPUT:
            headers = {"Input", "Captured"};
            rows = {{"0", "0"}, {"1", "1"}};
            return;
        case ComponentType::CLOCK:
            headers = {"Info"};
            rows = {{"Toggles on each"}, {"simulation tick"}};
            return;
        case ComponentType::LED:
            headers = {"Input", "Light"};
            rows = {{"0", "OFF"}, {"1", "ON"}};
            return;
        case ComponentType::SR_FF:
            headers = {"S", "R", "Q(next)"};
            rows = {{"0", "0", "Q (hold)"}, {"0", "1", "0"}, {"1", "0", "1"}, {"1", "1", "Invalid"}};
            return;
        case ComponentType::D_FF:
            headers = {"D", "Q(next)"};
            rows = {{"0", "0"}, {"1", "1"}};
            return;
        case ComponentType::JK_FF:
            headers = {"J", "K", "Q(next)"};
            rows = {{"0", "0", "Q (hold)"}, {"0", "1", "0"}, {"1", "0", "1"}, {"1", "1", "Toggle"}};
            return;
        case ComponentType::T_FF:
            headers = {"T", "Q(next)"};
            rows = {{"0", "Q (hold)"}, {"1", "Toggle"}};
            return;
        case ComponentType::CUSTOM: {
            const auto* def = CustomComponentManager::getInstance().getDefinition(customTypeName);
            if (def && def->numInputs > 8) {
                headers = {"Info"};
                rows.push_back({"Inputs: " + std::to_string(def->numInputs)});
                rows.push_back({"Outputs: " + std::to_string(def->numOutputs)});
                rows.push_back({"(Table too large)"});
                return;
            }
            break;
        }
        default:
            break;
    }
    
    if (!previewComponent) return;
    
    int numInputs = static_cast<int>(previewComponent->getInputs().size());
    int numOutputs = static_cast<int>(previewComponent->getOutputs().size());
    
    for (int i = 0; i < numInputs; i++) {
        headers.push_back(std::string(1, 'A' + i));
    }
    if (numOutputs == 1) {
        headers.push_back("OUT");
    } else {
        for (int i = 0; i < numOutputs; i++) {
            headers.push_back("O" + std::to_string(i));
        }
    }
    
    int numRows = 1 << numInputs;
    for (int r = 0; r < numRows; r++) {
        std::vector<std::string> row;
        
        const auto& inputs = previewComponent->getInputs();
        for (int i = 0; i < numInputs; i++) {
            bool val = (r >> (numInputs - 1 - i)) & 1;
            row.push_back(val ? "1" : "0");
            inputs[i]->setValue(val ? 1 : 0);
        }
        
        previewComponent->calculate();
        
        const auto& outputs = previewComponent->getOutputs();
        for (int i = 0; i < numOutputs; i++) {
            int outputVal = outputs[i]->getValue();
            row.push_back(outputVal >= 1 ? "1" : "0");
        }
        
        rows.push_back(row);
    }

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
            int numInputsInRow = 0;
            for (const auto& h : headers) {
                if (h.length() == 1 && h[0] >= 'A' && h[0] <= 'Z') numInputsInRow++;
                else break;
            }
            
            for (size_t i = 0; i < row.size(); i++) {
                ImGui::TableSetColumnIndex(static_cast<int>(i));
                if (static_cast<int>(i) >= numInputsInRow && row[i] == "1") {
                    ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.2f, 1.0f), "%s", row[i].c_str());
                } else {
                    ImGui::Text("%s", row[i].c_str());
                }
            }
        }
        ImGui::EndTable();
    }
}
