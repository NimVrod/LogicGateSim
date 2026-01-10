#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

#include "../Simulation/Circuit.h"
#include "ComponentPreview.h"

class UIManager
{
public:
    UIManager(sf::RenderWindow& window, Circuit& circuit);
    ~UIManager() = default;

    void Init();
    void Update(sf::Time dt);
    void Render();
    void ProcessEvent(const sf::Event& event);

    // Getters for some specific main loop needs if necessary, 
    // though we try to keep logic inside.
    bool ShouldClose() const { return shouldClose; }

private:
    sf::RenderWindow& window;
    Circuit& circuit;
    ComponentPreview componentPreview;

    // Window visibility
    bool showComponentPicker = true;
    bool showSimulationControl = true;
    bool showComponentList = true;
    bool showComponentPreview = false;
    bool showOpenConfirmPopup = false;

    // Simulation control
    float simulationInterval = 0.0f;
    float timeAccumulator = 0.0f;
    bool simulationRunning = false;

    // Display options
    bool shouldDrawPins = false;
    bool shouldDrawLabels = false;

    // Component picker state
    int numberInputs = 2;
    int selectedGate = 0;
    int selectedInput = 0;
    int selectedOutput = 0;
    int selectedCategory = 0; // 0=Gates, 1=Inputs, 2=Outputs

    // File operations
    std::string currentFilePath = "circuit.xml";
    std::string statusMessage;
    float statusMessageTimer = 0.0f;
    const float STATUS_MESSAGE_DURATION = 3.0f;
    std::string pendingOpenPath;

    // View control (Zoom/Pan handled in main for the View itself, but UI controls it too)
    // We might need a callback or reference to the view, or simply access window view.
    // Ideally, main.cpp handles the actual View manipulation based on events, 
    // but the UI buttons also need to affect it.
    // For now, let's just modify the view directly via the window if possible or pass it in.
    // Wait, modifying window.setView from here affects the render state.
    // Let's pass the circuitView as a reference if we want to button-control it, 
    // or just let the user control it via mouse in main.
    // Actually, the Zoom In/Out buttons in UI need access to the view.
    // Let's add methods to access and modify the view.
    
    // Internal Draw Methods
    void DrawMenuBar();
    void DrawComponentPicker();
    void DrawSimulationControl();
    void DrawComponentList();
    void DrawContextMenu();
    void DrawStatus();
    void LoadTheme();

    // Helper for adding components
    void AddGate(int gateType, sf::Vector2f pos);
    void AddInput(int inputType, sf::Vector2f pos);

    bool shouldClose = false;

    // Static arrays for combos
    static const char* gateTypes[];
    static const char* inputTypes[];
    static const char* outputTypes[];
};
