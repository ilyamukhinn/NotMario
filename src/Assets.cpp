#include "Assets.hpp"
#include "Enums.hpp"

#include <cassert>
#include <iostream>
#include <fstream>

void Assets::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    std::string str{ "" };
    while (file.good()) {
        file >> str;

        switch (strToAssetTag.at(str))
        {
        case AssetTag::TEXTURE: {
            std::string name{ "" }, path{ "" };
            file >> name >> path;
            this->addTexture(name, path);
            break;
        }
        case AssetTag::ANIMATION: {
            std::string name{ "" }, texture{ "" };
            size_t frames{ 0 }, speed{ 0 };
            file >> name >> texture >> frames >> speed;
            this->addAnimation(strToAnimationTag.at(name), texture, frames, speed);
            break;
        }
        case AssetTag::FONT: {
            std::string name{ "" }, path{ "" };
            file >> name >> path;
            this->addFont(name, path);
            break;
        }
        default:
            std::cerr << "Unknown Asset Type: " << str << std::endl;
            break;
        }
    }
}

void Assets::addTexture(const std::string& textureName, const std::string& path, bool smooth) {
    this->m_textureMap[textureName] = sf::Texture();

    if (!this->m_textureMap[textureName].loadFromFile(path)) {
        std::cerr << "Could not load texture file: " << path << std::endl;
        this->m_textureMap.erase(textureName);
    } else {
        this->m_textureMap[textureName].setSmooth(smooth);
        std::cout << "Loaded Texture: " << path << std::endl;
    }
}

const sf::Texture& Assets::getTexture(const std::string& textureName) const {
    assert(this->m_textureMap.find(textureName) != this->m_textureMap.end());
    return this->m_textureMap.at(textureName);
}

void Assets::addAnimation(const AnimationTag& animationName,
    const std::string& textureName, size_t frameCount, size_t speed) {
    if (this->m_animationMap.find(animationName) != this->m_animationMap.end()) {
        std::cerr << "Animation '" << animationTagToStr.at(animationName) << "' already exists\n";
    } else {
        this->m_animationMap[animationName] = Animation(animationName,
            this->getTexture(textureName), frameCount, speed);
    }
}

const Animation& Assets::getAnimation(const AnimationTag& animationName) const {
    assert(this->m_animationMap.find(animationName) != this->m_animationMap.end());
    return this->m_animationMap.at(animationName);
}

void Assets::addFont(const std::string& fontName, const std::string& path) {
    this->m_fontMap[fontName] = sf::Font();

    if (!this->m_fontMap[fontName].loadFromFile(path)) {
        std::cerr << "Could not load font file: " << path << std::endl;
        this->m_fontMap.erase(fontName);
    } else {
        std::cout << "Loaded Font: " << path << std::endl;
    }
}

const sf::Font& Assets::getFont(const std::string& fontName) const {
    assert(this->m_fontMap.find(fontName) != this->m_fontMap.end());
    return this->m_fontMap.at(fontName);
}
