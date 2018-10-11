#pragma once

#include <SFML/Graphics.hpp>
#include "IsometricDrawable.hpp"
#include <vector>

class IsometricDrawer
{
    sf::RenderTexture image;

    sf::Shader isoShader;
    std::uint32_t isoShaderIndexColor;
    std::uint32_t isoShaderIndexNormal;
    std::uint32_t isoShaderIndexPosition;

    struct DrawData
    {
        sf::Vector2f const& position;
        IsometricDrawable const& drawable;
    };
    std::vector<DrawData> drawDatas;
public:
    IsometricDrawer();
    void Add(sf::Vector2f const& position, IsometricDrawable const& drawable);
    void Render(sf::Vector2f const& cameraPos, sf::Vector3f const& lightPos);
    sf::Texture const& Texture();
};