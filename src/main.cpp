#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <nfd.h>
#include "Core/ResourceManager.h"
#include "Simulation/Circuit.h"
#include "UI/UIManager.h"

int main()
{
    NFD_Init();

    sf::ContextSettings contextSettings;
    contextSettings.antiAliasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Logic Gate Simulator", sf::Style::Default, sf::State::Windowed, contextSettings);
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window))
    {
        NFD_Quit();
        return -1;
    }

    ResourceManager& resourceManager = ResourceManager::getInstance();

    Circuit circuit;
    UIManager uiManager(window, circuit);
    uiManager.Init();

    // View navigation
    sf::View circuitView = window.getDefaultView();
    bool isPanning = false;
    sf::Vector2i lastMousePos;
    float zoomLevel = 1.0f;

    sf::Clock deltaClock;

    while (window.isOpen())
    {
        if (uiManager.ShouldClose())
        {
            window.close();
        }

        // Set circuit view for correct coordinate mapping during events
        window.setView(circuitView);
        
        // === EVENT POLLING ===
        while (const auto event = window.pollEvent())
        {
            ImGui::SFML::ProcessEvent(window, *event);
            uiManager.ProcessEvent(*event);

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (!ImGui::GetIO().WantCaptureMouse)
            {
                // Handle view panning with middle mouse button
                if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mousePressed->button == sf::Mouse::Button::Middle) {
                        isPanning = true;
                        lastMousePos = sf::Mouse::getPosition(window);
                    }
                }
                else if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
                    if (mouseReleased->button == sf::Mouse::Button::Middle) {
                        isPanning = false;
                    }
                }
                else if (event->is<sf::Event::MouseMoved>()) {
                    if (isPanning) {
                        sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
                        sf::Vector2f delta = window.mapPixelToCoords(lastMousePos) - window.mapPixelToCoords(currentMousePos);
                        circuitView.move(delta);
                        window.setView(circuitView);
                        lastMousePos = currentMousePos;
                    }
                }
                else if (const auto* mouseScrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
                    // Zoom with mouse wheel
                    if (mouseScrolled->wheel == sf::Mouse::Wheel::Vertical) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        sf::Vector2f worldPosBefore = window.mapPixelToCoords(mousePos);
                        
                        float zoomFactor = (mouseScrolled->delta > 0) ? 0.9f : 1.1f;
                        zoomLevel *= zoomFactor;
                        circuitView.zoom(zoomFactor);
                        window.setView(circuitView);
                        
                        // Zoom towards mouse position
                        sf::Vector2f worldPosAfter = window.mapPixelToCoords(mousePos);
                        circuitView.move(worldPosBefore - worldPosAfter);
                        window.setView(circuitView);
                    }

                     if (mouseScrolled->wheel == sf::Mouse::Wheel::Horizontal) {
                        // Move view left/right on horizontal scroll
                        sf::Vector2f panAmount = (mouseScrolled->delta > 0) ? sf::Vector2f(-20.f, 0.f) : sf::Vector2f(20.f, 0.f);
                        circuitView.move(panAmount);
                        window.setView(circuitView);
                    }
                }
                
                circuit.handleEvent(*event, window);
            }
        }

        // === UPDATE ===
        const sf::Time dt = deltaClock.restart();
        
        // Update main window ImGui
        ImGui::SFML::Update(window, dt);

        uiManager.Update(dt);

        // === RENDERING ===
        // Background color matched to UIManager's theme or personal preference
        // Using a slightly lighter dark grey than pure black for contrast
        window.clear(sf::Color(20, 20, 20));
        
        // Set circuit view for drawing
        window.setView(circuitView);
        circuit.draw(window);
        
        // Reset to default view for ImGui
        window.setView(window.getDefaultView());
        
        // Render UI
        uiManager.Render();
        
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    NFD_Quit();
    return 0;
}