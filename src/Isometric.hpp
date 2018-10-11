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
        sf::Vector2f unitX = sf::Transform().rotate(-(float)std::atan(2.0f)/pi*180).transformPoint(sf::Vector2f(1.0f, 0.0f));
        sf::Vector2f unitY = sf::Transform().rotate(-(float)std::atan(0.5f)/pi*180).transformPoint(sf::Vector2f(0.0f, 1.0f));
        sf::Vector2f result(
            position.x * unitX.x + position.y * unitX.y,
            position.x * unitY.x + position.y * unitY.y
        );
        result *= -1.0f;
        return result + sf::Vector2f((float)screenSize.x, (float)screenSize.y)/2.0f;
    }

    static sf::Vector2f ScreenposToRealpos(sf::Vector2f const& screenPos, sf::Vector2f const& cameraPos, sf::Vector2u const& screenSize)
    {
        sf::Vector2f unitX = sf::Transform().rotate((float)std::atan(2.0f)/pi*180).transformPoint(sf::Vector2f(1.0f, 0.0f));
        sf::Vector2f unitY = sf::Transform().rotate((float)std::atan(0.5f)/pi*180).transformPoint(sf::Vector2f(0.0f, 1.0f));
        sf::Vector2f centered = screenPos - sf::Vector2f((float)screenSize.x, (float)screenSize.y)/2.0f;
        centered.y *= -1.0f;
        sf::Vector2f result(
            centered.x * unitX.x + centered.y * unitX.y,
            centered.x * unitY.x + centered.y * unitY.y
        );
        return result;
    }
};