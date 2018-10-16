#pragma once

#include "TwodDrawer.hpp"
#include "TwodText.hpp"
#include <optional>
#include "IsometricDrawer.hpp"
#include "TextureLoader.hpp"

class LevelEditor
{
    IsometricDrawer& isoDrawer;

    bool placing = false;
    sf::Vector2f placingTextPos;
    std::string placingTextString;
    TwodText placingTextDraw;

    TextureLoader& texLoader;

    sf::Texture colorMap;
    std::uint32_t normalMap;
    std::uint32_t positionMap;

    class Placeholder
    {
    public:
        sf::Vector3f position;
        IsometricDrawable drawable;
        Placeholder(IsometricDrawable drawable) : drawable(drawable) {}
    };

    std::optional<Placeholder> placeholder;
public:
    LevelEditor(sf::Font const& font, TwodDrawer& guiDrawer, IsometricDrawer& isoDrawer, TextureLoader& texLoader);
    void Update();
};