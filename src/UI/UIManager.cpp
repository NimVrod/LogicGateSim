#include "UIManager.h"
#include <nfd.h>
#include "../Serialization/CircuitSerializer.h"
#include "../Core/ResourceManager.h"

// Component Includes
#include "../Core/Components/Include/Button.h"
#include "../Core/Components/Include/AndGate.h"
#include "../Core/Components/Include/OrGate.h"
#include "../Core/Components/Include/NotGate.h"
#include "../Core/Components/Include/NandGate.h"
#include "../Core/Components/Include/NorGate.h"
#include "../Core/Components/Include/XorGate.h"
#include "../Core/Components/Include/XnorGate.h"

const char* UIManager::gateTypes[] = { "AND", "OR", "NOT", "NAND", "NOR", "XOR", "XNOR" };
const char* UIManager::inputTypes[] = { "Button" };
const char* UIManager::outputTypes[] = { "(None available)" };

UIManager::UIManager(sf::RenderWindow& window, Circuit& circuit)
    : window(window), circuit(circuit)
{
}

void UIManager::Init()
{
    LoadTheme();
}

void UIManager::LoadTheme()
{
    // Check if a font is already loaded by checking the atlas
    // If we want to set a specific font, we would do it here or in main.
    // ImGui::SFML::Init handles the default font.
    
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Modern Dark Theme
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

void UIManager::Update(sf::Time dt)
{
    // Update ImGui
    // ImGui::SFML::Update(window, dt); // logic handled in main, we just do drawing commands here
    // Wait, the main loop calls ImGui::SFML::Update. 
    // We should call the draw methods here.

    // Simulation Timer Logic
    timeAccumulator += dt.asSeconds();
    if (simulationRunning)
    {
        if (simulationInterval <= 0.0f)
        {
            circuit.update();
        }
        else if (timeAccumulator >= simulationInterval)
        {
            circuit.update();
            timeAccumulator = 0.0f;
        }
    }
    
    // Status Message Timer
    if (!statusMessage.empty())
    {
        statusMessageTimer += dt.asSeconds();
        if (statusMessageTimer >= STATUS_MESSAGE_DURATION)
        {
            statusMessage.clear();
            statusMessageTimer = 0.0f;
        }
    }
}

void UIManager::Render()
{
    DrawMenuBar();
    
    if (showComponentPicker) DrawComponentPicker();
    if (showSimulationControl) DrawSimulationControl();
    if (showComponentList) DrawComponentList();
    if (showComponentPreview) 
    {
        componentPreview.show();
    }
    else
    {
        componentPreview.hide();
    }
    componentPreview.renderUI();

    DrawContextMenu();
    DrawStatus();
}

void UIManager::ProcessEvent(const sf::Event& event)
{
    // ImGui events processed in main
}

void UIManager::DrawMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {
                nfdu8char_t* outPath = nullptr;
                nfdu8filteritem_t filters[1] = { { "Circuit Files", "xml" } };
                nfdsavedialogu8args_t args = {0};
                args.filterList = filters;
                args.filterCount = 1;
                args.defaultName = "circuit.xml";
                
                nfdresult_t result = NFD_SaveDialogU8_With(&outPath, &args);
                if (result == NFD_OKAY)
                {
                    currentFilePath = outPath;
                    if (CircuitSerializer::saveToFile(circuit, currentFilePath)) {
                        statusMessage = "Saved: " + currentFilePath;
                        statusMessageTimer = 0.0f;
                    } else {
                        statusMessage = "Failed to save circuit.";
                        statusMessageTimer = 0.0f;
                    }
                    NFD_FreePathU8(outPath);
                }
            }
            
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                nfdu8char_t* outPath = nullptr;
                nfdu8filteritem_t filters[1] = { { "Circuit Files", "xml" } };
                nfdopendialogu8args_t args = {nullptr};
                args.filterList = filters;
                args.filterCount = 1;
                
                nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &args);
                if (result == NFD_OKAY)
                {
                    pendingOpenPath = outPath;
                    NFD_FreePathU8(outPath);
                    showOpenConfirmPopup = true;
                }
            }
            
            ImGui::Separator();
            
            if (ImGui::MenuItem("Exit"))
            {
                shouldClose = true;
            }
            
            ImGui::EndMenu();
        }
        
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Component Picker", nullptr, &showComponentPicker);
            ImGui::MenuItem("Simulation Control", nullptr, &showSimulationControl);
            ImGui::MenuItem("Component List", nullptr, &showComponentList);
            ImGui::MenuItem("Component Preview", nullptr, &showComponentPreview);
            ImGui::Separator();
            if (ImGui::MenuItem("Show All"))
            {
                showComponentPicker = true;
                showSimulationControl = true;
                showComponentList = true;
                showComponentPreview = true;
            }
            if (ImGui::MenuItem("Hide All"))
            {
                showComponentPicker = false;
                showSimulationControl = false;
                showComponentList = false;
                showComponentPreview = false;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // Modal Popup
    if (showOpenConfirmPopup)
    {
        ImGui::OpenPopup("Confirm Open");
    }
    
    if (ImGui::BeginPopupModal("Confirm Open", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Opening a new circuit will overwrite the current one.");
        ImGui::Text("Are you sure you want to continue?");
        ImGui::Separator();
        
        if (ImGui::Button("Open", ImVec2(120, 0)))
        {
            if (CircuitSerializer::loadFromFile(circuit, pendingOpenPath)) {
                currentFilePath = pendingOpenPath;
                statusMessage = "Loaded: " + currentFilePath;
                statusMessageTimer = 0.0f;
            } else {
                statusMessage = "Failed to load circuit.";
                statusMessageTimer = 0.0f;
            }
            showOpenConfirmPopup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0)))
        {
            showOpenConfirmPopup = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}

void UIManager::DrawComponentPicker()
{
    ImGui::Begin("Component Picker", &showComponentPicker);
    
    if (ImGui::BeginTabBar("ComponentCategories"))
    {
        if (ImGui::BeginTabItem("Gates"))
        {
            selectedCategory = 0;
            ImGui::Combo("Gate Type", &selectedGate, gateTypes, IM_ARRAYSIZE(gateTypes));
            
            if (selectedGate != 2) // NOT is index 2
            {
                ImGui::SliderInt("Number of Inputs", &numberInputs, 2, 8);
            }
            
            if (ImGui::Button("Add Gate", ImVec2(-1, 0)))
            {
                AddGate(selectedGate, sf::Vector2f(100, 100));
            }
            
            // Update preview
            if (selectedCategory == 0)
            {
                int inputs = (selectedGate == 2) ? 1 : numberInputs;
                componentPreview.setComponentType(gateTypes[selectedGate], inputs);
            }
            
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Inputs"))
        {
            selectedCategory = 1;
            ImGui::Combo("Input Type", &selectedInput, inputTypes, IM_ARRAYSIZE(inputTypes));
            
            if (ImGui::Button("Add Input", ImVec2(-1, 0)))
            {
                AddInput(selectedInput, sf::Vector2f(100, 100));
            }
            
            // Update preview
            if (selectedCategory == 1)
            {
                componentPreview.setComponentType(inputTypes[selectedInput], 1);
            }
            
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Outputs"))
        {
            selectedCategory = 2;
            ImGui::TextDisabled("No output components available yet.");
            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar();
    }
    
    ImGui::Separator();
    
    if (ImGui::Button(componentPreview.isVisible() ? "Hide Preview" : "Show Preview"))
    {
        componentPreview.toggle();
    }
    
    ImGui::Separator();
    
    if (ImGui::Button("Clear All", ImVec2(-1, 0)))
    {
        circuit.clear();
    }
    
    ImGui::End();
}

void UIManager::DrawSimulationControl()
{
    ImGui::Begin("Simulation Control", &showSimulationControl);
    ImGui::Checkbox("Run Simulation", &simulationRunning);
    if (ImGui::Button("Step"))
    {
        circuit.update();
    }
    ImGui::SliderFloat("Update Interval (s)", &simulationInterval, 0.0f, 2.0f, "%.2f");
    if (simulationInterval == 0.0f)
    {
        ImGui::Text("Mode: Every frame");
    }
    else
    {
        ImGui::Text("Mode: Every %.2f seconds", simulationInterval);
    }
    ImGui::Separator();
    bool prevDrawPins = shouldDrawPins;
    bool prevDrawLabels = shouldDrawLabels;
    ImGui::Checkbox("Draw all pins", &shouldDrawPins);
    ImGui::Checkbox("Draw all labels", &shouldDrawLabels);
    
    if (prevDrawPins != shouldDrawPins) circuit.setDrawAllPins(shouldDrawPins);
    if (prevDrawLabels != shouldDrawLabels) circuit.setDrawLabels(shouldDrawLabels);
    
    ImGui::Separator();
    ImGui::Text("Grid:");
    bool showGrid = circuit.getShowGrid();
    if (ImGui::Checkbox("Show Grid", &showGrid)) {
        circuit.setShowGrid(showGrid);
    }
    bool snapToGrid = circuit.getSnapToGrid();
    if (ImGui::Checkbox("Snap to Grid", &snapToGrid)) {
        circuit.setSnapToGrid(snapToGrid);
    }
    float gridSize = circuit.getGridSize();
    if (ImGui::SliderFloat("Grid Size", &gridSize, 10.0f, 50.0f, "%.0f")) {
        circuit.setGridSize(gridSize);
    }
    
    ImGui::Separator();
    ImGui::Text("View Control:");
    // Simple buttons to zoom/reset view - these need to affect the main view
    // Since we don't have direct access to the View object here unless we pass it or store it,
    // we can assume the user handles zoom via mouse, OR we can pass it in.
    // For now, let's omit the buttons if we can't easily access the view, OR 
    // we can change the UIManager signature to accept the view in Render() or Update().
    // Actually, let's keep it simple. The mouse wheel zoom is superior.
    // But for completeness, we can add them back if we refactor `Render` to take `sf::View&`.
    ImGui::TextDisabled("(Use Mouse Wheel to Zoom/Pan)");
    
    ImGui::End();
}

void UIManager::DrawComponentList()
{
    ImGui::Begin("Component List", &showComponentList);
    const auto& components = circuit.GetComponents();
    for (int i = 0; i < static_cast<int>(components.size()); i++)
    {
        ImGui::PushID(i);
        ImGui::Text("%s", components[i]->GetLabel().c_str());
        ImGui::SameLine();
        if (ImGui::Button("Delete"))
        {
            circuit.removeComponent(components[i]->GetId());
        }
        ImGui::SameLine();
        if (ImGui::Button("Go to"))
        {
            // circuit.GotoComponent(components[i]->GetId(), window);
            // This requires window to change view. 
            // Luckily we have window reference.
            circuit.GotoComponent(components[i]->GetId(), window);
        }
        ImGui::PopID();
    }
    ImGui::End();
}

void UIManager::DrawContextMenu()
{
    static int contextMenuTargetId = -1;
    if (circuit.getContextMenuComponentId() != -1)
    {
        contextMenuTargetId = circuit.getContextMenuComponentId();
        circuit.clearContextMenu();
        ImGui::OpenPopup("ComponentContextMenu");
    }
    if (ImGui::BeginPopup("ComponentContextMenu"))
    {
        if (ImGui::MenuItem("Delete"))
        {
            circuit.removeComponent(contextMenuTargetId);
            contextMenuTargetId = -1;
        }
        ImGui::EndPopup();
    }
}

void UIManager::DrawStatus()
{
    if (!statusMessage.empty())
    {
        ImGui::Begin("Status", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("%s", statusMessage.c_str());
        ImGui::End();
    }
}

void UIManager::AddGate(int gateType, sf::Vector2f pos)
{
    switch (gateType)
    {
        case 0: circuit.addComponent(std::make_unique<AndGate>(circuit.getNextId(), pos, numberInputs)); break;
        case 1: circuit.addComponent(std::make_unique<OrGate>(circuit.getNextId(), pos, numberInputs)); break;
        case 2: circuit.addComponent(std::make_unique<NotGate>(circuit.getNextId(), pos)); break;
        case 3: circuit.addComponent(std::make_unique<NandGate>(circuit.getNextId(), pos, numberInputs)); break;
        case 4: circuit.addComponent(std::make_unique<NorGate>(circuit.getNextId(), pos, numberInputs)); break;
        case 5: circuit.addComponent(std::make_unique<XorGate>(circuit.getNextId(), pos, numberInputs)); break;
        case 6: circuit.addComponent(std::make_unique<XnorGate>(circuit.getNextId(), pos, numberInputs)); break;
    }
}

void UIManager::AddInput(int inputType, sf::Vector2f pos)
{
    switch (inputType)
    {
        case 0: circuit.addComponent(std::make_unique<Button>(circuit.getNextId(), pos)); break;
    }
}
