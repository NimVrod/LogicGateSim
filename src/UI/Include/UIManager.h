#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

#include "../../Simulation/Circuit.h"
#include "ComponentPreview.h"
#include "PinPlotter.h"

class UIManager {
public:
    static UIManager &getInstance();

    UIManager(const UIManager &) = delete;

    UIManager &operator=(const UIManager &) = delete;

    void Init(sf::RenderWindow &window, Circuit &circuit);

    void Update(sf::Time dt);

    void Render();

    bool ShouldClose() const { return shouldClose; }

    void setStatusMessage(const std::string &message);

private:
    UIManager() = default;

    ~UIManager() = default;

    sf::RenderWindow *window = nullptr;
    Circuit *circuit = nullptr;
    ComponentPreview componentPreview;
    PinPlotter pinPlotter;

    bool showComponentPicker = true;
    bool showSimulationControl = true;
    bool showComponentList = true;
    bool showComponentPreview = false;
    bool showPinPlotter = false;
    bool showOpenConfirmPopup = false;
    bool showSaveAsCustomPopup = false;

    float simulationInterval = 0.0f;
    float timeAccumulator = 0.0f;
    bool simulationRunning = false;
    bool clocksEnabled = true;

    bool shouldDrawPins = false;
    bool shouldDrawLabels = false;

    int numberInputs = 2;
    int selectedGate = 0;
    int selectedInput = 0;
    int selectedOutput = 0;
    int selectedOther = 0;
    int selectedCustom = 0;
    int selectedCategory = 0;

    std::string currentFilePath = "circuit.xml";
    std::string statusMessage;
    float statusMessageTimer = 0.0f;
    const float STATUS_MESSAGE_DURATION = 3.0f;
    std::string pendingOpenPath;
    std::string customComponentName;
    std::string customComponentsFile = "custom_components.xml";

    void DrawMenuBar();

    void DrawComponentPicker();

    void DrawSimulationControl();

    void DrawComponentList();

    void DrawContextMenu();

    void DrawStatus();

    void LoadTheme();

    void AddGate(int gateType, sf::Vector2f pos);

    void AddInput(int inputType, sf::Vector2f pos);

    void AddOutput(int outputType, sf::Vector2f pos);

    void AddOther(int otherType, sf::Vector2f pos);

    void AddCustomComponent(const std::string &name, sf::Vector2f pos);

    bool shouldClose = false;

    static const char *gateTypes[];
    static const char *inputTypes[];
    static const char *outputTypes[];
    static const char *otherTypes[];
};