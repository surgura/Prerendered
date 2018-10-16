#include "TwodDrawer.hpp"
#include <GL/glew.h>

#include <iostream>
#include <chrono>

TwodDrawer::TwodDrawer()
{
    sf::ContextSettings settings;
    if (!image.create(800, 600, settings))
    {
        std::cout << "TwodDrawer init error!" << std::endl;
        // error...
    }
}

void TwodDrawer::Add(TwodDrawable const& drawable)
{
    drawDatas.push_back({drawable});
}

void TwodDrawer::Render()
{
    glDisable(GL_DEPTH_TEST);
    glUseProgram(0);
    image.clear(sf::Color::Transparent);
    
    for (auto const& drawData : drawDatas)
    {
        drawData.drawable.Draw(image);
    }

    image.display();
}

sf::Texture const& TwodDrawer::Texture()
{
    return image.getTexture();
}