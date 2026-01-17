#include "../Include/UIManager.h"
#include <nfd.h>
#include "../../Serialization/CircuitSerializer.h"
#include "../../Core/Components/Include/ResourceManager.h"
#include "../../Core/Components/Include/ComponentFactory.h"
#include "../../Core/Components/CustomComponent/CustomComponent.h"
#include "../../Core/Components/CustomComponent/CustomComponentManager.h"
#include "../../Core/Components/CustomComponent/CustomComponentDefinition.h"

const char* UIManager::gateTypes[] = { "AND", "OR", "NOT", "NAND", "NOR", "XOR", "XNOR" };
const char* UIManager::inputTypes[] = { "Button", "Input Pin", "Clock" };
const char* UIManager::outputTypes[] = { "Output Pin", "LED" };
const char* UIManager::otherTypes[] = { "SR FF", "D FF", "JK FF", "T FF" };

UIManager& UIManager::getInstance() {
    static UIManager instance;
    return instance;
}

void UIManager::Init(sf::RenderWindow& win, Circuit& circ) {
    window = &win;
    circuit = &circ;
    LoadTheme();
    CustomComponentManager::getInstance().loadFromFile(customComponentsFile);
}

void UIManager::LoadTheme() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    style.WindowRounding = 8.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 5.0f;
    style.ScrollbarRounding = 12.0f;
    style.GrabRounding = 5.0f;
    style.TabRounding = 5.0f;
    
    style.Colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.95f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.22f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.24f, 0.24f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void UIManager::Update(sf::Time dt) {
    timeAccumulator += dt.asSeconds();
    if (simulationRunning) {
        if (simulationInterval <= 0.0f) {
            static int frameCounter = 0;
            frameCounter++;
            if (clocksEnabled && frameCounter >= 30) {
                circuit->tickClocks();
                frameCounter = 0;
            }
            circuit->update();
            pinPlotter.recordValues();
        }
        else if (timeAccumulator >= simulationInterval) {
            if (clocksEnabled)
                circuit->tickClocks();
            circuit->update();
            pinPlotter.recordValues();
            timeAccumulator = 0.0f;
        }
    }
    
    if (!statusMessage.empty()) {
        statusMessageTimer += dt.asSeconds();
        if (statusMessageTimer >= STATUS_MESSAGE_DURATION) {
            statusMessage.clear();
            statusMessageTimer = 0.0f;
        }
    }
}

void UIManager::Render() {
    DrawMenuBar();
    
    if (showComponentPicker) DrawComponentPicker();
    if (showSimulationControl) DrawSimulationControl();
    if (showComponentList) DrawComponentList();
    if (showComponentPreview != componentPreview.isVisible()) {
        if (showComponentPreview) componentPreview.show();
        else componentPreview.hide();
    }
    
    componentPreview.renderUI();
    showComponentPreview = componentPreview.isVisible();
    
    if (showPinPlotter != pinPlotter.isVisible()) {
        if (showPinPlotter) pinPlotter.show();
        else pinPlotter.hide();
    }
    pinPlotter.renderUI();
    showPinPlotter = pinPlotter.isVisible();

    DrawContextMenu();
    DrawStatus();
}

void UIManager::DrawMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                nfdu8char_t* outPath = nullptr;
                nfdu8filteritem_t filters[1] = { { "Circuit Files", "xml" } };
                nfdsavedialogu8args_t args = {0};
                args.filterList = filters;
                args.filterCount = 1;
                args.defaultName = "circuit.xml";
                
                nfdresult_t result = NFD_SaveDialogU8_With(&outPath, &args);
                if (result == NFD_OKAY) {
                    currentFilePath = outPath;
                    if (CircuitSerializer::saveToFile(*circuit, currentFilePath)) {
                        setStatusMessage("Saved: " + currentFilePath);
                    } else {
                        setStatusMessage("Failed to save circuit.");
                    }
                    NFD_FreePathU8(outPath);
                }
            }
            
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                nfdu8char_t* outPath = nullptr;
                nfdu8filteritem_t filters[1] = { { "Circuit Files", "xml" } };
                nfdopendialogu8args_t args = {nullptr};
                args.filterList = filters;
                args.filterCount = 1;
                
                nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
                if (result == NFD_OKAY) {
                    pendingOpenPath = outPath;
                    NFD_FreePathU8(outPath);
                    showOpenConfirmPopup = true;
                }
            }
            
            if (ImGui::MenuItem("Save As Custom Component")) {
                bool hasInputs = false;
                bool hasOutputs = false;
                for (const auto& comp : circuit->GetComponents()) {
                    if (comp->getType() == "Input Pin") hasInputs = true;
                    if (comp->getType() == "Output Pin") hasOutputs = true;
                }
                
                if (!hasInputs && !hasOutputs) {
                    setStatusMessage("Add Input/Output components to define the interface.");
                } else {
                    customComponentName.clear();
                    showSaveAsCustomPopup = true;
                }
            }
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("Exit")) {
                shouldClose = true;
            }
            
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("Component Picker", nullptr, &showComponentPicker);
            ImGui::MenuItem("Simulation Control", nullptr, &showSimulationControl);
            ImGui::MenuItem("Component List", nullptr, &showComponentList);
            ImGui::MenuItem("Component Preview", nullptr, &showComponentPreview);
            ImGui::MenuItem("Pin Plotter", nullptr, &showPinPlotter);
            ImGui::Separator();
            if (ImGui::MenuItem("Show All")) {
                showComponentPicker = true;
                showSimulationControl = true;
                showComponentList = true;
                showComponentPreview = true;
                showPinPlotter = true;
            }
            if (ImGui::MenuItem("Hide All")) {
                showComponentPicker = false;
                showSimulationControl = false;
                showComponentList = false;
                showComponentPreview = false;
                showPinPlotter = false;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (showOpenConfirmPopup) {
        ImGui::OpenPopup("Confirm Open");
    }
    
    if (ImGui::BeginPopupModal("Confirm Open", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Opening a new circuit will overwrite the current one.");
        ImGui::Text("Are you sure you want to continue?");
        ImGui::Separator();
        
        if (ImGui::Button("Open", ImVec2(120, 0))) {
            if (CircuitSerializer::loadFromFile(*circuit, pendingOpenPath)) {
                currentFilePath = pendingOpenPath;
                setStatusMessage("Loaded: " + currentFilePath);
            } else {
                setStatusMessage("Failed to load circuit.");
            }
            showOpenConfirmPopup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            showOpenConfirmPopup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    
    if (showSaveAsCustomPopup) {
        ImGui::OpenPopup("Save As Custom Component");
    }
    
    if (ImGui::BeginPopupModal("Save As Custom Component", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Enter a name for your custom component:");
        
        static char nameBuffer[64] = "";
        if (showSaveAsCustomPopup) {
            strcpy(nameBuffer, customComponentName.c_str());
            showSaveAsCustomPopup = false;
        }
        
        ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer));
        
        int inputCount = 0, outputCount = 0;
        for (const auto& comp : circuit->GetComponents()) {
            if (comp->getType() == "Input Pin") inputCount++;
            if (comp->getType() == "Output Pin") outputCount++;
        }
        ImGui::Text("Inputs: %d, Outputs: %d", inputCount, outputCount);
        
        ImGui::Separator();
        
        if (ImGui::Button("Save", ImVec2(120, 0))) {
            std::string name = nameBuffer;
            if (!name.empty()) {
                CustomComponentDefinition def;
                def.name = name;
                def.circuitXml = CircuitSerializer::saveToXmlString(*circuit);
                def.numInputs = inputCount;
                def.numOutputs = outputCount;
                
                if (CustomComponentManager::getInstance().addDefinition(def)) {
                    CustomComponentManager::getInstance().saveToFile(customComponentsFile);
                    setStatusMessage("Saved custom component: " + name);
                } else {
                    setStatusMessage("Component '" + name + "' already exists.");
                }
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void UIManager::DrawComponentPicker() {
    ImGui::Begin("Component Picker", &showComponentPicker);
    
    if (ImGui::BeginTabBar("ComponentCategories")) {
        if (ImGui::BeginTabItem("Gates")) {
            selectedCategory = 0;
            ImGui::Combo("Gate Type", &selectedGate, gateTypes, IM_ARRAYSIZE(gateTypes));
            
            if (selectedGate != 2) {
                ImGui::SliderInt("Number of Inputs", &numberInputs, 2, 8);
            }
            
            if (ImGui::Button("Add Gate", ImVec2(-1, 0))) {
                AddGate(selectedGate, sf::Vector2f(100, 100));
            }

            int inputs = (selectedGate == 2) ? 1 : numberInputs;
            static const char* gateTypeNames[] = { "AndGate", "OrGate", "NotGate", "NandGate", "NorGate", "XorGate", "XnorGate" };
            if (selectedGate >= 0 && selectedGate < 7) {
                componentPreview.setComponentType(gateTypeNames[selectedGate], inputs);
            }

            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Inputs")) {
            selectedCategory = 1;
            ImGui::Combo("Input Type", &selectedInput, inputTypes, IM_ARRAYSIZE(inputTypes));
            
            if (ImGui::Button("Add Input", ImVec2(-1, 0))) {
                AddInput(selectedInput, sf::Vector2f(100, 100));
            }
            static const char* inputTypeNames[] = { "Button", "Input Pin", "ClockComponent" };
            if (selectedInput >= 0 && selectedInput < 3) {
                componentPreview.setComponentType(inputTypeNames[selectedInput], 1);
            }

            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Outputs")) {
            selectedCategory = 2;
            ImGui::Combo("Output Type", &selectedOutput, outputTypes, IM_ARRAYSIZE(outputTypes));
            
            if (ImGui::Button("Add Output", ImVec2(-1, 0))) {
                AddOutput(selectedOutput, sf::Vector2f(100, 100));
            }

            static const char* outputTypeNames[] = { "Output Pin", "LEDComponent" };
            if (selectedOutput >= 0 && selectedOutput < 2) {
                componentPreview.setComponentType(outputTypeNames[selectedOutput], 1);
            }

            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Other")) {
            selectedCategory = 3;
            ImGui::Combo("Component Type", &selectedOther, otherTypes, IM_ARRAYSIZE(otherTypes));
            
            if (ImGui::Button("Add Component", ImVec2(-1, 0))) {
                AddOther(selectedOther, sf::Vector2f(100, 100));
            }

            static const char* otherTypeNames[] = { "SRFlipFlop", "DFlipFlop", "JKFlipFlop", "TFlipFlop" };
            if (selectedOther >= 0 && selectedOther < 4) {
                componentPreview.setComponentType(otherTypeNames[selectedOther], 1);
            }

            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Custom")) {
            selectedCategory = 4;
            
            auto names = CustomComponentManager::getInstance().getDefinitionNames();
            if (names.empty()) {
                ImGui::TextDisabled("No custom components available.");
                ImGui::TextDisabled("Use File > Save As Custom Component.");
            } else {
                std::string comboItems;
                for (const auto& name : names) {
                    comboItems += name;
                    comboItems += '\0';
                }
                comboItems += '\0';
                
                if (selectedCustom >= static_cast<int>(names.size())) {
                    selectedCustom = 0;
                }
                
                ImGui::Combo("Component", &selectedCustom, comboItems.c_str());
                
                const auto* def = CustomComponentManager::getInstance().getDefinition(names[selectedCustom]);
                if (def) {
                    ImGui::Text("Inputs: %d, Outputs: %d", def->numInputs, def->numOutputs);
                }
                
                if (ImGui::Button("Add Component", ImVec2(-1, 0))) {
                    AddCustomComponent(names[selectedCustom], sf::Vector2f(100, 100));
                }
                
                componentPreview.setComponentType(names[selectedCustom], def ? def->numInputs : 0);
            }
            
            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar();
    }
    
    ImGui::Separator();
    
    if (ImGui::Button(componentPreview.isVisible() ? "Hide Preview" : "Show Preview"))
        showComponentPreview = !showComponentPreview;

    ImGui::Separator();
    
    if (ImGui::Button("Clear All", ImVec2(-1, 0))) {
        circuit->clear();
    }
    
    ImGui::End();
}

void UIManager::DrawSimulationControl() {
    ImGui::Begin("Simulation Control", &showSimulationControl);
    ImGui::Checkbox("Run Simulation", &simulationRunning);
    ImGui::SameLine();
    if (ImGui::Button("Step"))
        circuit->update();
    ImGui::SliderFloat("Update Interval (s)", &simulationInterval, 0.0f, 2.0f, "%.2f");
    if (simulationInterval == 0.0f)
        ImGui::Text("Mode: Every frame");
    else
        ImGui::Text("Mode: Every %.2f seconds", simulationInterval);

    ImGui::Separator();
    ImGui::Text("Clocks:");
    ImGui::Checkbox("Enable Clocks", &clocksEnabled);
    ImGui::SameLine();
    if (ImGui::Button("Tick Clocks")) {
        circuit->tickClocks();
        circuit->update();
    }
    
    ImGui::Separator();
    bool prevDrawPins = shouldDrawPins;
    bool prevDrawLabels = shouldDrawLabels;
    ImGui::Checkbox("Draw all pins", &shouldDrawPins);
    ImGui::Checkbox("Draw all labels", &shouldDrawLabels);
    
    if (prevDrawPins != shouldDrawPins) circuit->setDrawAllPins(shouldDrawPins);
    if (prevDrawLabels != shouldDrawLabels) circuit->setDrawLabels(shouldDrawLabels);
    
    ImGui::Separator();
    ImGui::Text("Grid:");
    bool showGrid = circuit->getShowGrid();
    if (ImGui::Checkbox("Show Grid", &showGrid)) {
        circuit->setShowGrid(showGrid);
    }
    bool snapToGrid = circuit->getSnapToGrid();
    if (ImGui::Checkbox("Snap to Grid", &snapToGrid)) {
        circuit->setSnapToGrid(snapToGrid);
    }
    float gridSize = circuit->getGridSize();
    if (ImGui::SliderFloat("Grid Size", &gridSize, 10.0f, 50.0f, "%.0f")) {
        circuit->setGridSize(gridSize);
    }
    
    ImGui::Separator();
    ImGui::Text("View Control:");
    ImGui::TextDisabled("(Use Mouse Wheel to Zoom/Pan)");
    
    ImGui::End();
}

void UIManager::DrawComponentList() {
    ImGui::Begin("Component List", &showComponentList);
    const auto& components = circuit->GetComponents();
    for (int i = 0; i < static_cast<int>(components.size()); i++) {
        ImGui::PushID(i);
        ImGui::Text("%s", components[i]->GetLabel().c_str());
        ImGui::SameLine();
        if (ImGui::Button("Delete")) {
            circuit->removeComponent(components[i]->GetId());
        }
        ImGui::SameLine();
        if (ImGui::Button("Go to")) {
            circuit->GotoComponent(components[i]->GetId(), *window);
        }
        ImGui::PopID();
    }
    ImGui::End();
}

void UIManager::DrawContextMenu() {
    static int contextMenuTargetId = -1;
    if (circuit->getContextMenuComponentId() != -1) {
        contextMenuTargetId = circuit->getContextMenuComponentId();
        circuit->clearContextMenu();
        ImGui::OpenPopup("ComponentContextMenu");
    }
    if (ImGui::BeginPopup("ComponentContextMenu")) {
        Component* targetComp = circuit->getComponentById(contextMenuTargetId);
        
        if (ImGui::MenuItem("Delete")) {
            circuit->removeComponent(contextMenuTargetId);
            contextMenuTargetId = -1;
        }
        
        if (targetComp && ImGui::BeginMenu("Plot Pin...")) {
            const auto& inputs = targetComp->getInputs();
            const auto& outputs = targetComp->getOutputs();
            
            bool hasPins = !inputs.empty() || !outputs.empty();
            bool atMaxCapacity = pinPlotter.getPinCount() >= PinPlotter::MAX_PINS;
            if (atMaxCapacity) {
                ImGui::TextDisabled("Max %zu pins monitored", PinPlotter::MAX_PINS);
                ImGui::Separator();
            }
            if (!inputs.empty()) {
                ImGui::TextDisabled("Inputs:");
                for (size_t i = 0; i < inputs.size(); ++i) {
                    std::string label = targetComp->GetLabel() + " IN" + std::to_string(i);
                    if (ImGui::MenuItem(label.c_str(), nullptr, false, !atMaxCapacity)) {
                        if (pinPlotter.addPin(inputs[i].get(), label)) {
                            showPinPlotter = true;
                        }
                    }
                }
            }
            if (!outputs.empty()) {
                if (!inputs.empty()) ImGui::Separator();
                ImGui::TextDisabled("Outputs:");
                for (size_t i = 0; i < outputs.size(); ++i) {
                    std::string label = targetComp->GetLabel() + " OUT" + std::to_string(i);
                    if (ImGui::MenuItem(label.c_str(), nullptr, false, !atMaxCapacity)) {
                        if (pinPlotter.addPin(outputs[i].get(), label)) {
                            showPinPlotter = true;
                        }
                    }
                }
            }
            if (!hasPins)
                ImGui::TextDisabled("No pins available");

            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }
}

void UIManager::DrawStatus() {
    if (!statusMessage.empty()) {
        ImGui::Begin("Status", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("%s", statusMessage.c_str());
        ImGui::End();
    }
}

void UIManager::AddGate(int gateType, sf::Vector2f pos) {
    static const char* gateTypeMap[] = {
        "AndGate", "OrGate", "NotGate",
        "NandGate", "NorGate", "XorGate", "XnorGate"
    };
    if (gateType >= 0 && gateType < 7) {
        circuit->addComponent(ComponentFactory::create(gateTypeMap[gateType], circuit->getNextId(), pos, numberInputs));
    }
}

void UIManager::AddInput(int inputType, sf::Vector2f pos) {
    static const char* inputTypeMap[] = {
        "Button", "Input Pin", "ClockComponent"
    };
    if (inputType >= 0 && inputType < 3) {
        int thirdParam = 2; // Default for gates
        if (inputType == 1) { // Input Pin
            // Calculate the next index based on existing Input Pins
            int inputIndex = 0;
            for (const auto& comp : circuit->GetComponents()) {
                if (comp->getType() == "Input Pin") {
                    inputIndex++;
                }
            }
            thirdParam = inputIndex;
        }
        circuit->addComponent(ComponentFactory::create(inputTypeMap[inputType], circuit->getNextId(), pos, thirdParam));
    }
}

void UIManager::AddOutput(int outputType, sf::Vector2f pos) {
    static const char* outputTypeMap[] = {
        "Output Pin", "LEDComponent"
    };
    if (outputType >= 0 && outputType < 2) {
        int thirdParam = 2; // Default
        if (outputType == 0) { // Output Pin
            // Calculate the next index based on existing Output Pins
            int outputIndex = 0;
            for (const auto& comp : circuit->GetComponents()) {
                if (comp->getType() == "Output Pin") {
                    outputIndex++;
                }
            }
            thirdParam = outputIndex;
        }
        circuit->addComponent(ComponentFactory::create(outputTypeMap[outputType], circuit->getNextId(), pos, thirdParam));
    }
}

void UIManager::AddOther(int otherType, sf::Vector2f pos) {
    static const char* otherTypeMap[] = {
        "SRFlipFlop", "DFlipFlop", "JKFlipFlop", "TFlipFlop"
    };
    if (otherType >= 0 && otherType < 4) {
        circuit->addComponent(ComponentFactory::create(otherTypeMap[otherType], circuit->getNextId(), pos, 2));
    }
}

void UIManager::AddCustomComponent(const std::string& name, sf::Vector2f pos) {
    circuit->addComponent(ComponentFactory::createCustom(circuit->getNextId(), pos, name));
}

void UIManager::setStatusMessage(const std::string& message) {
    statusMessage = message;
    statusMessageTimer = 0.0f;
}
