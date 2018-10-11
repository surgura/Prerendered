#include "IsometricDrawer.hpp"
#include <GL/glew.h>
#include "Isometric.hpp"

#include <iostream>
#include <chrono>

IsometricDrawer::IsometricDrawer()
{
    sf::ContextSettings settings;
    settings.depthBits         = 24; // Request a 24 bits depth buffer
    settings.stencilBits       = 8;  // Request a 8 bits stencil buffer
    settings.antialiasingLevel = 2;  // Request 2 levels of antialiasing
    if (!image.create(800, 600, settings))
    {
        std::cout << "IsometricDrawer init error!" << std::endl;
        // error...
    }

    if (!isoShader.loadFromFile("lighting.vert", "lighting.frag"))
    {
        std::cout << "Could not load iso shader!" << std::endl;
    }

    isoShaderIndexColor = glGetUniformLocation(isoShader.getNativeHandle(), "colormap");
    if (isoShaderIndexColor == -1)
    {
        std::cout << "Could not find color!" << std::endl;
    }
    isoShaderIndexNormal = glGetUniformLocation(isoShader.getNativeHandle(), "normalmap");
    if (isoShaderIndexNormal == -1)
    {
        std::cout << "Could not find normal!" << std::endl;
    }
    isoShaderIndexDepth = glGetUniformLocation(isoShader.getNativeHandle(), "depthmap");
    if (isoShaderIndexDepth == -1)
    {
        std::cout << "Could not find depth!" << std::endl;
    }
}

void IsometricDrawer::Add(sf::Vector2f const& position, IsometricDrawable const& drawable)
{
    drawDatas.push_back({position, drawable});
}

uint64_t unix_timestamp()
{
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch()
    );
    return ms.count();
}

void IsometricDrawer::Render(sf::Vector2f const& cameraPos)
{
    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);                             // The Type Of Depth Testing To Do
    glDepthRange(0.0f, 1.0f);
    glClearDepth(1.0f);                                 // Depth Buffer Setup

    image.clear(sf::Color::Blue);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // move light as a test
    std::uint64_t unixtime = unix_timestamp();
    std::uint64_t botpart = unixtime;
    botpart /= 1000000;
    unixtime -= botpart * 1000000;
    float movespeed = 0.002f;
    sf::Vector3f lightpos(5+10.0f*(float)std::cos(((float)unixtime)*movespeed), 5, 5);
    isoShader.setUniform("lightpos", lightpos);

    sf::Shader::bind(&isoShader);
    
    for (auto const& drawData : drawDatas)
    {
        sf::Sprite sprite;
        sprite.setPosition(Isometric::RealposToScreenpos(drawData.position, cameraPos) + sf::Vector2f((float)image.getSize().x, (float)image.getSize().y)/2.0f);
        sprite.setTexture(drawData.drawable.colorMap);
        sprite.setOrigin(drawData.drawable.origin);

        glUniform1i(isoShaderIndexNormal, 1);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, drawData.drawable.normalMap);
        glActiveTexture(GL_TEXTURE0);
        
        glUniform1i(isoShaderIndexDepth, 2);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, drawData.drawable.depthMap);
        glActiveTexture(GL_TEXTURE0);

        image.draw(sprite);
    }

    image.display();
}

sf::Texture const& IsometricDrawer::Texture()
{
    return image.getTexture();
}