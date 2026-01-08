#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

#include "Core/ResourceManager.h"
#include "Simulation/Circuit.h"
#include "Core/Components/Include/Button.h"
#include "Core/Components/Include/AndGate.h"
#include "Core/Components/Include/OrGate.h"
#include "Core/Components/Include/NotGate.h"
#include "Core/Components/Include/NandGate.h"
#include "Core/Components/Include/NorGate.h"
#include "Core/Components/Include/XorGate.h"
#include "Core/Components/Include/XnorGate.h"
#include "Serialization/CircuitSerializer.h"

int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.antiAliasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Logic Gate Simulator", sf::Style::Default, sf::State::Windowed, contextSettings);
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window))
        return -1;

    Circuit circuit;
    bool simulationRunning = false;
    bool shouldDrawPins = false;
    bool shouldDrawLabels = false;
    sf::Clock deltaClock;
    int numberInputs = 2;
    window.requestFocus();
    deltaClock.restart(); // Initialize clock before loop
    ResourceManager &resourceManager = ResourceManager::getInstance();

    // File path for save/load
    static char filePath[256] = "circuit.xml";
    static std::string statusMessage = "";


    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            
            //Block handleEvent over imgui
            if (!ImGui::GetIO().WantCaptureMouse) {
                circuit.handleEvent(*event, window);
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        if (numberInputs > 8) numberInputs = 8;
        if (numberInputs < 1) numberInputs = 1;

        // UI
        ImGui::Begin("Component Picker");
        if (ImGui::Button("Add button")) {
            circuit.addComponent(std::make_unique<Button>(circuit.getNextId(), sf::Vector2f(100, 100)));
        }
        if (ImGui::Button("Add AND gate")) {
            circuit.addComponent(std::make_unique<AndGate>(circuit.getNextId(), sf::Vector2f(100, 100), numberInputs));
		}
		if (ImGui::Button("Add OR gate")) {
            circuit.addComponent(std::make_unique<OrGate>(circuit.getNextId(), sf::Vector2f(100, 100), numberInputs));
        }
        if (ImGui::Button("Add NOT gate")) {
            circuit.addComponent(std::make_unique<NotGate>(circuit.getNextId(), sf::Vector2f(100, 100)));
        }
        if (ImGui::Button("Add NAND gate")) {
            circuit.addComponent(std::make_unique<NandGate>(circuit.getNextId(), sf::Vector2f(100, 100), numberInputs));
        }
        if (ImGui::Button("Add NOR gate")) {
            circuit.addComponent(std::make_unique<NorGate>(circuit.getNextId(), sf::Vector2f(100, 100), numberInputs));
        }
        if (ImGui::Button("Add XOR gate")) {
            circuit.addComponent(std::make_unique<XorGate>(circuit.getNextId(), sf::Vector2f(100, 100), numberInputs));
        }
        if (ImGui::Button("Add XNOR gate")) {
            circuit.addComponent(std::make_unique<XnorGate>(circuit.getNextId(), sf::Vector2f(100, 100), numberInputs));
        }
        ImGui::InputInt("Number of gates", &numberInputs);
        ImGui::Separator();
        if (ImGui::Button("Clear All")) {
            circuit.clear();
        }
        ImGui::End();

        //TODO: Maybe use native file dialogs instead of text input for file paths?

        // File Operations Window
        ImGui::Begin("File Operations");
        ImGui::InputText("File Path", filePath, sizeof(filePath));
        if (ImGui::Button("Save")) {
            if (CircuitSerializer::saveToFile(circuit, filePath)) {
                statusMessage = "Circuit saved successfully!";
            } else {
                statusMessage = "Failed to save circuit.";
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Load")) {
            if (CircuitSerializer::loadFromFile(circuit, filePath)) {
                statusMessage = "Circuit loaded successfully!";
            } else {
                statusMessage = "Failed to load circuit.";
            }
        }
        if (!statusMessage.empty()) {
            ImGui::Text("%s", statusMessage.c_str());
        }
        ImGui::End();

        ImGui::Begin("Simulation Control");
        ImGui::Checkbox("Run Simulation", &simulationRunning);
        if (ImGui::Button("Step")) {
            circuit.update();
        }
        ImGui::Checkbox("Draw all pins", &shouldDrawPins);
        ImGui::Checkbox("Draw all labels", &shouldDrawLabels);
        ImGui::End();

        ImGui::Begin("Component list");
        const auto& components = circuit.GetComponents();
        for (int i = 0; i < components.size(); i++)
        {
            ImGui::PushID(i);
            ImGui::Text("%s", components[i]->GetLabel().c_str());
            ImGui::SameLine();
            if (ImGui::Button("Delete")) {
                circuit.removeComponent(components[i]->GetId());
            }
            ImGui::SameLine();
            if (ImGui::Button("Go to")) {
                circuit.GotoComponent(components[i]->GetId(), window);
            }
            ImGui::PopID();
        }

        ImGui::End();

        if (simulationRunning) {
            circuit.update();
        }

        window.clear(sf::Color(20, 20, 20)); // Dark background
        
        circuit.setDrawAllPins(shouldDrawPins);
        circuit.setDrawLabels(shouldDrawLabels);
        circuit.draw(window);
        
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}