#include "LevelEditor.hpp"
#include <SFML/Graphics.hpp>

LevelEditor::LevelEditor(sf::Font const& font, TwodDrawer& guiDrawer, IsometricDrawer& isoDrawer, TextureLoader& texLoader) :
    isoDrawer(isoDrawer),
    placingTextString(""),
    placingTextDraw(font, placingTextPos, placingTextString),
    texLoader(texLoader)
{
    guiDrawer.Add(placingTextDraw);

    colorMap = *texLoader.GetColorMap("cube.color.png");
    normalMap = *texLoader.GetNormalMap("cube.normal.exr");
    positionMap = *texLoader.GetPositionMap("cube.position.exr");
}

void LevelEditor::Update()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M) && !placing)
    {
        placing = true;

        placeholder.emplace(IsometricDrawable(colorMap, normalMap, positionMap));
        isoDrawer.Add(placeholder->position, placeholder->drawable);
    }
}