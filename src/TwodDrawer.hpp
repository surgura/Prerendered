#pragma once

#include <SFML/Graphics.hpp>
#include "TwodDrawable.hpp"
#include <vector>

class TwodDrawer
{
    sf::RenderTexture image;

    struct DrawData
    {
        TwodDrawable const& drawable;
    };
    std::vector<DrawData> drawDatas;
public:
    TwodDrawer();
    void Add(TwodDrawable const& drawable);
    void Render();
    sf::Texture const& Texture();
};