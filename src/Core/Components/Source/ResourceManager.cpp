#include "../Include/ResourceManager.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>


ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

sf::Texture& ResourceManager::getTexture(const std::string& filename) {
    auto it = textures.find(filename);
    if (it != textures.end()) {
        return it->second;
    } else {
        sf::Texture texture;
        if (texture.loadFromFile(filename)) {
            textures[filename] = texture;
            return textures[filename];
        } else {
            throw std::runtime_error("Failed to load texture: " + filename);
        }
    }
}

sf::Font& ResourceManager::getFont(const std::string& filename) {
    auto it = fonts.find(filename);
    if (it != fonts.end()) {
        return it->second;
    } else {
        sf::Font font;
        if (font.openFromFile(filename)) {
            fonts[filename] = font;
            return fonts[filename];
        } else {
            throw std::runtime_error("Failed to load font: " + filename);
        }
    }
}