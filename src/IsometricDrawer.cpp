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
    isoShaderIndexPosition = glGetUniformLocation(isoShader.getNativeHandle(), "positionmap");
    if (isoShaderIndexPosition == -1)
    {
        std::cout << "Could not find position!" << std::endl;
    }
}

void IsometricDrawer::Add(sf::Vector3f const& position, IsometricDrawable const& drawable)
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

void IsometricDrawer::Render(sf::Vector2f const& cameraPos, sf::Vector3f const& lightPos)
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glDepthRange(0.0f, 1.0f);
    glClearDepth(1.0f);

    image.clear(sf::Color::Blue);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // move light as a test
    std::uint64_t unixtime = unix_timestamp();
    std::uint64_t botpart = unixtime;
    botpart /= 1000000;
    unixtime -= botpart * 1000000;
    float movespeed = 0.002f;
    //sf::Vector3f lightpos(5+10.0f*(float)std::cos(((float)unixtime)*movespeed), 5, 5);
    isoShader.setUniform("lightpos", lightPos);
    
    for (auto const& drawData : drawDatas)
    {
        isoShader.setUniform("position", drawData.position);
        sf::Shader::bind(&isoShader);

        sf::Sprite sprite;
        sprite.setPosition(Isometric::RealposToScreenpos3d(drawData.position, cameraPos, image.getSize()));
        sprite.setTexture(drawData.drawable.colorMap);
        sprite.setOrigin(drawData.drawable.origin);
        sprite.setScale(drawData.drawable.scale);

        glUniform1i(isoShaderIndexNormal, 1);
        glActiveTexture(GL_TEXTURE0 + 1);
        glBindTexture(GL_TEXTURE_2D, drawData.drawable.normalMap);
        glActiveTexture(GL_TEXTURE0);
        
        glUniform1i(isoShaderIndexPosition, 2);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, drawData.drawable.positionMap);
        glActiveTexture(GL_TEXTURE0);

        image.draw(sprite);
    }

    image.display();
}

sf::Texture const& IsometricDrawer::Texture()
{
    return image.getTexture();
}