#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>

class IsometricDrawable
{
public:
    IsometricDrawable(sf::Texture colorMap, std::uint32_t normalMap, std::uint32_t depthMap) :
        colorMap(colorMap),
        normalMap(normalMap),
        depthMap(depthMap)
    {}

    sf::Texture colorMap;
    std::uint32_t normalMap;
    std::uint32_t depthMap;
    sf::Vector2f origin;
};