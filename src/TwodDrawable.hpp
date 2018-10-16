#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>

class TwodDrawable
{
public:
    virtual void Draw(sf::RenderTarget& renderTarget) const = 0;
};