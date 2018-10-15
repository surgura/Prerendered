#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

#include <iostream>

class Isometric
{
    static constexpr float pi = 3.141592653589793238462643383279502884f;
public:
    static sf::Vector2f RealposToScreenpos(sf::Vector2f const& position, sf::Vector2f const& cameraPos, sf::Vector2u const& screenSize)
    {
        sf::Vector2f result = position;
        
        sf::Vector2f unitX(0.70710678118f, 0.70710678118f);
        sf::Vector2f unitY(0.70710678118f, -0.70710678118f);
        result = {
            result.x * unitX.x + result.y * unitX.y,
            result.x * unitY.x + result.y * unitY.y
        };
        result.y *= 0.5f;
        result += sf::Vector2f((float)screenSize.x, (float)screenSize.y)/2.0f;
        return result;
    }

    static sf::Vector2f ScreenposToRealpos(sf::Vector2f const& screenPos, sf::Vector2f const& cameraPos, sf::Vector2u const& screenSize)
    {
        sf::Vector2f centered = screenPos - sf::Vector2f((float)screenSize.x, (float)screenSize.y)/2.0f;
        centered.y *= 2.0f;

        // 0.5 sqrt(2)
        sf::Vector2f unitX(0.70710678118f, 0.70710678118f);
        sf::Vector2f unitY(0.70710678118f, -0.70710678118f);
        sf::Vector2f result(
            centered.x * unitX.x + centered.y * unitX.y,
            centered.x * unitY.x + centered.y * unitY.y
        );
        return result;
    }
};