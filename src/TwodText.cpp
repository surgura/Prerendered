#include "TwodText.hpp"

TwodText::TwodText(sf::Font const& font, sf::Vector2f const& position, std::string const& text) :
    font(font),
    position(position),
    text(text)
{}

void TwodText::Draw(sf::RenderTarget& renderTarget) const
{
    sf::Text text(text, font);
    text.setPosition(position);
    renderTarget.draw(text);
}