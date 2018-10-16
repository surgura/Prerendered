#pragma once

#include "TwodDrawable.hpp"

class TwodText : public TwodDrawable
{
    sf::Font const& font;
    sf::Vector2f const& position;
    std::string const& text;
public:
    TwodText(sf::Font const& font, sf::Vector2f const& position, std::string const& text);
    void Draw(sf::RenderTarget& renderTarget) const override;
};