#pragma once

#ifndef IMGUI_SFML_EXAMPLE_RESOURCEMANAGER_H
#define IMGUI_SFML_EXAMPLE_RESOURCEMANAGER_H

#include <SFML/Graphics/Texture.hpp>

#include "SFML/Graphics/Font.hpp"

class ResourceManager {
private:
    ResourceManager() = default;
    std::unordered_map<std::string, sf::Texture> textures_;
    std::unordered_map<std::string, sf::Font> fonts_;
public:
    static ResourceManager& getInstance();
    sf::Texture& getTexture(const std::string& filename);
    sf::Font& getFont(const std::string& filename);
};


#endif //IMGUI_SFML_EXAMPLE_RESOURCEMANAGER_H