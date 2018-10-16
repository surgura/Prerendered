#pragma once

#include "TwodDrawable.hpp"

class TwodText : public TwodDrawable
{
    sf::Font const& font;
    sf::Vector2f const& position;
public:
    TwodText(sf::Font const& font, sf::Vector2f const& position);
    void Draw(sf::RenderTarget& renderTarget) const override;
};