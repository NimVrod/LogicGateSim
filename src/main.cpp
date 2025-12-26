#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include "Simulation/Circuit.h"
#include "Core/Components/Include/Button.h"
#include "Core/Components/Include/AndGate.h"
#include "Core/Components/Include/OrGate.h"
#include "Core/Components/Include/NotGate.h"
#include "Core/Components/Include/NandGate.h"
#include "Core/Components/Include/NorGate.h"
#include "Core/Components/Include/XorGate.h"
#include "Core/Components/Include/XnorGate.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Logic Gate Simulator");
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window))
        return -1;

    Circuit circuit;
    bool simulationRunning = false;
    sf::Clock deltaClock;
    int numberInputs = 2;
    window.requestFocus();
    deltaClock.restart(); // Initialize clock before loop

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            
            // Pass event to circuit for Interaction (if mapped to window coords)
            // But we might want to block circuit interaction if mouse is over ImGui?
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
            circuit.addComponent(std::make_unique<Button>(sf::Vector2f(100, 100)));
        }
        if (ImGui::Button("Add AND gate")) {
            circuit.addComponent(std::make_unique<AndGate>(sf::Vector2f(100, 100), numberInputs));
		}
		if (ImGui::Button("Add OR gate")) {
            circuit.addComponent(std::make_unique<OrGate>(sf::Vector2f(100, 100), numberInputs));
        }
        if (ImGui::Button("Add NOT gate")) {
            circuit.addComponent(std::make_unique<NotGate>(sf::Vector2f(100, 100)));
        }
        if (ImGui::Button("Add NAND gate")) {
            circuit.addComponent(std::make_unique<NandGate>(sf::Vector2f(100, 100), numberInputs));
        }
        if (ImGui::Button("Add NOR gate")) {
            circuit.addComponent(std::make_unique<NorGate>(sf::Vector2f(100, 100), numberInputs));
        }
        if (ImGui::Button("Add XOR gate")) {
            circuit.addComponent(std::make_unique<XorGate>(sf::Vector2f(100, 100), numberInputs));
        }
        if (ImGui::Button("Add XNOR gate")) {
            circuit.addComponent(std::make_unique<XnorGate>(sf::Vector2f(100, 100), numberInputs));
        }
        ImGui::InputInt("Number of gates", &numberInputs);
        ImGui::Separator();
        if (ImGui::Button("Clear All")) {
            circuit.clear();
        }
        ImGui::End();

        ImGui::Begin("Simulation Control");
        ImGui::Checkbox("Run Simulation", &simulationRunning);
        if (ImGui::Button("Step")) {
            circuit.update();
        }
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
            ImGui::PopID();
        }

        ImGui::End();

        if (simulationRunning) {
            circuit.update();
        }

        window.clear(sf::Color(20, 20, 20)); // Dark background
        
        circuit.draw(window);
        
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}