#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>

class IsometricDrawable
{
public:
    IsometricDrawable(sf::Texture colorMap, std::uint32_t normalMap, std::uint32_t positionMap) :
        colorMap(colorMap),
        normalMap(normalMap),
        positionMap(positionMap)
    {}

    sf::Texture colorMap;
    std::uint32_t normalMap;
    std::uint32_t positionMap;
    sf::Vector2f origin;
};