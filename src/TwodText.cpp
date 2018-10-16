#include "TwodText.hpp"

TwodText::TwodText(sf::Font const& font, sf::Vector2f const& position) :
    font(font),
    position(position)
{}

void TwodText::Draw(sf::RenderTarget& renderTarget) const
{
    sf::Text text("hello", font);
    text.setPosition(position);
    renderTarget.draw(text);
}