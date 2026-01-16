#pragma once
#include <SFML/Graphics/Texture.hpp>
#include "SFML/Graphics/Font.hpp"

class ResourceManager {
public:
    static constexpr const char* DEFAULT_FONT = "assets/ARIAL.TTF";
    
    static ResourceManager& getInstance();
    sf::Texture& getTexture(const std::string& filename);
    sf::Font& getFont(const std::string& filename);
    sf::Font& getDefaultFont() { return getFont(DEFAULT_FONT); }
    
private:
    ResourceManager() = default;
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
};

