#pragma once

#include <SFML/System/Vector2.hpp>

class Isometric
{
public:
    static sf::Vector2f RealposToScreenpos(sf::Vector2f const& position, sf::Vector2f const& cameraPos)
    {
        sf::Transform fromIsoToScreen;
        fromIsoToScreen.rotate(45);
        sf::Vector2f screenPos = fromIsoToScreen.transformPoint(position - sf::Vector2f(-cameraPos.x, cameraPos.y));
        screenPos.y *= 0.5;
        return -screenPos;
    }

    static sf::Vector2f ScreenposToRealpos(sf::Vector2f const& screenPos, sf::Vector2f const& cameraPos, sf::Vector2u const& screenSize)
    {
        sf::Vector2f result = screenPos;
        result -= sf::Vector2f((float)screenSize.x, (float)screenSize.y)/2.0f;
        result *= -1.0f;
        result.y *= 2.0f;
        sf::Transform rotate;
        rotate.rotate(-45);
        result = rotate.transformPoint(result);
        result += sf::Vector2f(-cameraPos.x, cameraPos.y);
        return result;
    }
};