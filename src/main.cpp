#include <iostream>

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <nfd.h>
#include "Core/Components/Include/ResourceManager.h"
#include "Simulation/Circuit.h"
#include "UI/Include/UIManager.h"

int main() {
    const float MAX_ZOOM_IN = 0.1f;
    const float MAX_ZOOM_OUT = 10.0f;

    if (NFD_Init() == NFD_ERROR) {
        std::cerr << "Failed to initialize NFD\n";
        return -1;
    }

    sf::ContextSettings contextSettings;
    contextSettings.antiAliasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Symulator logiczny", sf::Style::Default, sf::State::Windowed,
                            contextSettings);
    window.setFramerateLimit(60);
    if (!ImGui::SFML::Init(window)) {
        NFD_Quit();
        return -1;
    }

    // No polish characters in default font
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    static const ImWchar polishRanges[] = {
        0x0020, 0x00FF,
        0x0100, 0x017F,
        0,
    };
    io.Fonts->AddFontFromFileTTF("assets/ARIAL.TTF", 15.0f, nullptr, polishRanges);
    if (!ImGui::SFML::UpdateFontTexture()) {
        std::cerr << "Failed to update ImGui font texture\n";
        ImGui::SFML::Shutdown();
        NFD_Quit();
        return -1;
    }

    ResourceManager &resourceManager = ResourceManager::getInstance();

    Circuit circuit;
    UIManager::getInstance().Init(window, circuit);

    sf::View circuitView = window.getDefaultView();
    bool isPanning = false;
    sf::Vector2i lastMousePos;
    float zoomLevel = 1.0f;

    sf::Clock deltaClock;

    while (window.isOpen()) {
        if (UIManager::getInstance().ShouldClose())
            window.close();

        window.setView(circuitView);

        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (!ImGui::GetIO().WantCaptureMouse) {
                if (const auto *mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mousePressed->button == sf::Mouse::Button::Middle) {
                        isPanning = true;
                        lastMousePos = sf::Mouse::getPosition(window);
                    }
                } else if (const auto *mouseReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
                    if (mouseReleased->button == sf::Mouse::Button::Middle) {
                        isPanning = false;
                    }
                } else if (event->is<sf::Event::MouseMoved>() && isPanning) {
                    sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
                    sf::Vector2f delta = window.mapPixelToCoords(lastMousePos) - window.mapPixelToCoords(
                                             currentMousePos);
                    circuitView.move(delta);
                    window.setView(circuitView);
                    lastMousePos = currentMousePos;
                } else if (const auto *mouseScrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
                    if (mouseScrolled->wheel == sf::Mouse::Wheel::Vertical) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        sf::Vector2f worldPosBefore = window.mapPixelToCoords(mousePos);

                        float zoomFactor = (mouseScrolled->delta > 0) ? 0.9f : 1.1f;
                        zoomLevel *= zoomFactor;
                        if (zoomLevel < MAX_ZOOM_IN) {
                            zoomLevel = MAX_ZOOM_IN;
                            zoomFactor = 1.0f;
                        } else if (zoomLevel > MAX_ZOOM_OUT) {
                            zoomLevel = MAX_ZOOM_OUT;
                            zoomFactor = 1.0f;
                        }
                        circuitView.zoom(zoomFactor);
                        window.setView(circuitView);

                        sf::Vector2f worldPosAfter = window.mapPixelToCoords(mousePos);
                        circuitView.move(worldPosBefore - worldPosAfter);
                        window.setView(circuitView);
                    } else if (mouseScrolled->wheel == sf::Mouse::Wheel::Horizontal) {
                        sf::Vector2f panAmount = (mouseScrolled->delta > 0)
                                                     ? sf::Vector2f(-20.f, 0.f)
                                                     : sf::Vector2f(20.f, 0.f);
                        circuitView.move(panAmount);
                        window.setView(circuitView);
                    }
                } else if (event->getIf<sf::Event::KeyPressed>()) {
                    // Using isKeyPressed to check real-time state, allowing diagonal panning
                    constexpr float panSpeed = 20.0f;
                    sf::Vector2f panAmount(0.f, 0.f);
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                        panAmount.x = -panSpeed;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                        panAmount.x = panSpeed;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                        panAmount.y = -panSpeed;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                        panAmount.y = panSpeed;
                    if (panAmount.x != 0.f || panAmount.y != 0.f) {
                        circuitView.move(panAmount);
                        window.setView(circuitView);
                    }
                }

                circuit.handleEvent(*event, window);
            }
        }

        const sf::Time dt = deltaClock.restart();
        ImGui::SFML::Update(window, dt);
        UIManager::getInstance().Update(dt);

        window.clear(sf::Color(20, 20, 20));

        window.setView(circuitView);
        circuit.draw(window);

        // Reset to default view for UI rendering
        window.setView(window.getDefaultView());
        UIManager::getInstance().Render();

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    NFD_Quit();
    return 0;
}