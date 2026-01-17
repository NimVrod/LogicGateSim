#include "../Include/ComponentPreview.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include <cmath>
#include <cfloat>
#include <unordered_map>
#include <unordered_set>

#include "../../Core/Components/Include/ComponentFactory.h"
#include "../../Core/Components/CustomComponent/CustomComponent.h"
#include "../../Core/Components/CustomComponent/CustomComponentManager.h"


ComponentPreview::ComponentPreview() {
    if (renderTexture.resize({200, 120})) {
        textureInitialized = true;
        renderTexture.setSmooth(true);
    }
}

void ComponentPreview::setComponentType(const std::string &typeName, int numInputs) {
    if (typeName != currentTypeName || numInputs != currentNumInputs) {
        currentTypeName = typeName;
        currentNumInputs = numInputs;
        updatePreviewComponent();
        generateTruthTable();
    }
}

void ComponentPreview::updatePreviewComponent() {
    previewComponent.reset();

    if (currentTypeName.empty()) return;

    sf::Vector2f origin(0.0f, 0.0f);
    previewComponent = ComponentFactory::create(currentTypeName, 0, origin, currentNumInputs);

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
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(FLT_MAX, FLT_MAX));

    if (ImGui::Begin("Podgląd komponentu", &visible)) {
        if (currentTypeName.empty()) {
            ImGui::TextDisabled("Wybierz komponent do podglądu");
        } else {
            ImGui::Text("Komponent: %s", currentTypeName.c_str());

            // Types that don't show input count
            static const std::unordered_set<std::string> singleInputTypes = {
                "Button", "NotGate", "ClockComponent", "LEDComponent",
                "SRFlipFlop", "DFlipFlop", "JKFlipFlop", "TFlipFlop",
                "Input Pin", "Output Pin"
            };

            if (singleInputTypes.find(currentTypeName) == singleInputTypes.end() && previewComponent) {
                ImGui::Text("Wejścia: %zu", previewComponent->getInputs().size());
            }

            ImGui::Separator();

            ImGui::Text("Podgląd:");
            if (ImGui::Checkbox("Pokaż piny", &drawPins)) {
                renderComponentToTexture();
            }
            if (textureInitialized && previewComponent) {
                ImGui::Image(renderTexture, sf::Color::White, sf::Color(80, 80, 80));
            } else {
                ImGui::TextDisabled("Podgląd niedostępny");
            }

            ImGui::Separator();

            ImGui::Text("Tablica prawdy:");
            renderTruthTableUI();
        }
    }
    ImGui::End();
}

void ComponentPreview::generateTruthTable() {
    headers.clear();
    rows.clear();

    if (currentTypeName.empty()) return;

    // Type-specific truth tables using unordered_map
    static const std::unordered_map<std::string, std::pair<std::vector<std::string>, std::vector<std::vector<
        std::string> > > > staticTables = {
        {"Button", {{"Stan", "OUT"}, {{"OFF", "0"}, {"ON", "1"}}}},
        {"Input Pin", {{"IN", "OUT"}, {{"0", "0"}, {"1", "1"}}}},
        {"Output Pin", {{"IN", "OUT"}, {{"0", "0"}, {"1", "1"}}}},
        {"ClockComponent", {{"Info"}, {{"-"}, {"-"}}}},
        {"LEDComponent", {{"Input", "ON/OFF"}, {{"0", "OFF"}, {"1", "ON"}}}},
        {
            "SRFlipFlop",
            {{"S", "R", "Q+"}, {{"0", "0", "Q"}, {"0", "1", "0"}, {"1", "0", "1"}, {"1", "1", "-"}}}
        },
        {"DFlipFlop", {{"D", "Q+"}, {{"0", "0"}, {"1", "1"}}}},
        {
            "JKFlipFlop",
            {{"J", "K", "Q+"}, {{"0", "0", "Q"}, {"0", "1", "0"}, {"1", "0", "1"}, {"1", "1", "-"}}}
        },
        {"TFlipFlop", {{"T", "Q+"}, {{"0", "Q"}, {"1", "Toggle"}}}}
    };

    auto staticIt = staticTables.find(currentTypeName);
    if (staticIt != staticTables.end()) {
        headers = staticIt->second.first;
        rows = staticIt->second.second;
        return;
    }

    // Check if it's a custom component with too many inputs
    if (CustomComponentManager::getInstance().hasDefinition(currentTypeName)) {
        const auto *def = CustomComponentManager::getInstance().getDefinition(currentTypeName);
        if (def && def->numInputs > 8) {
            headers = {"Info"};
            rows.push_back({"Wejścia: " + std::to_string(def->numInputs)});
            rows.push_back({"Wyjścia: " + std::to_string(def->numOutputs)});
            rows.push_back({"(Tablica zbyt duża)"});
            return;
        }
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

        const auto &inputs = previewComponent->getInputs();
        for (int i = 0; i < numInputs; i++) {
            bool val = (r >> (numInputs - 1 - i)) & 1;
            row.push_back(val ? "1" : "0");
            inputs[i]->setValue(val ? 1 : 0);
        }

        previewComponent->calculate();

        const auto &outputs = previewComponent->getOutputs();
        for (int i = 0; i < numOutputs; i++) {
            int outputVal = outputs[i]->getValue();
            row.push_back(outputVal >= 1 ? "1" : "0");
        }

        rows.push_back(row);
    }

    for (const auto &input: previewComponent->getInputs()) {
        input->setValue(0);
    }
    for (const auto &output: previewComponent->getOutputs()) {
        output->setValue(0);
    }
    previewComponent->calculate();
}

void ComponentPreview::renderTruthTableUI() {
    if (headers.empty()) return;

    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit;

    if (ImGui::BeginTable("TruthTable", static_cast<int>(headers.size()), flags)) {
        for (const auto &h: headers) {
            ImGui::TableSetupColumn(h.c_str(), ImGuiTableColumnFlags_WidthFixed, 30.0f);
        }
        ImGui::TableHeadersRow();

        for (const auto &row: rows) {
            ImGui::TableNextRow();
            int numInputsInRow = 0;
            for (const auto &h: headers) {
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