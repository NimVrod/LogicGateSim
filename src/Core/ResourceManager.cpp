#include "ResourceManager.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Font.hpp>


ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

sf::Texture& ResourceManager::getTexture(const std::string& filename) {
    auto it = textures_.find(filename);
    if (it != textures_.end()) {
        return it->second;
    } else {
        sf::Texture texture;
        if (texture.loadFromFile(filename)) {
            textures_[filename] = texture;
            return textures_[filename];
        } else {
            throw std::runtime_error("Failed to load texture: " + filename);
        }
    }
}

sf::Font& ResourceManager::getFont(const std::string& filename) {
    auto it = fonts_.find(filename);
    if (it != fonts_.end()) {
        return it->second;
    } else {
        sf::Font font;
        if (font.openFromFile(filename)) {
            fonts_[filename] = font;
            return fonts_[filename];
        } else {
            throw std::runtime_error("Failed to load font: " + filename);
        }
    }
}