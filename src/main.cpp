#include <GL/glew.h>
#include <iostream>
#include <optional>
#include "TextureLoader.hpp"
#include "IsometricDrawer.hpp"
#include "Isometric.hpp"
#include "TwodDrawer.hpp"
#include "TwodText.hpp"
#include "LevelEditor.hpp"

std::optional<IsometricDrawable> CreateCubeDrawable(TextureLoader& texLoader)
{
    auto colorMap = texLoader.GetColorMap("cube.color.png");
    if (!colorMap)
        return std::nullopt;
    auto normalMap = texLoader.GetNormalMap("cube.normal.exr");
    if (!normalMap)
        return std::nullopt;
    auto positionMap = texLoader.GetPositionMap("cube.position.exr");
    if (!positionMap)
        return std::nullopt;

    IsometricDrawable obj(*colorMap, *normalMap, *positionMap);
    obj.origin = { 512, 512 };

    return obj;
}

std::optional<IsometricDrawable> CreateTreeDrawable(TextureLoader& texLoader)
{
    auto colorMap = texLoader.GetColorMap("tree.color.png");
    if (!colorMap)
        return std::nullopt;
    auto normalMap = texLoader.GetNormalMap("tree.normal.exr");
    if (!normalMap)
        return std::nullopt;
    auto positionMap = texLoader.GetPositionMap("tree.position.exr");
    if (!positionMap)
        return std::nullopt;

    IsometricDrawable obj(*colorMap, *normalMap, *positionMap);
    obj.origin = { 512, 512 };

    return obj;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SFML OpenGL", sf::Style::Close);
    window.setFramerateLimit(30);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "glew init error" << std::endl;
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return -1;
    }

    TextureLoader texLoader(std::experimental::filesystem::path("tex"));

    sf::Vector2f cameraPos(0, 0);
    sf::Vector3f lightPos(-10,-300,400);


    IsometricDrawer isometricDrawer;

    auto drawable1 = CreateCubeDrawable(texLoader);
    if (!drawable1)
        return -1;
    sf::Vector3f position1;//{200,-200,0};

    auto drawable2 = CreateCubeDrawable(texLoader);
    if (!drawable2)
        return -1;
    sf::Vector3f position2;//{200,-450,0};
    float pos2zoffset = 0;

    isometricDrawer.Add(position1, *drawable1);
    isometricDrawer.Add(position2, *drawable2);


    TwodDrawer guiDrawer;

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
    {
        std::cout << "Could not load font" << std::endl;
        return -1;
    }

    LevelEditor levelEditor(font, guiDrawer, isometricDrawer, texLoader);

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        levelEditor.Update();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            cameraPos.x -= 10;
            cameraPos.y += 10;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            cameraPos.x += 10;
            cameraPos.y += 10;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            cameraPos.x += 10;
            cameraPos.y -= 10;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            cameraPos.x -= 10;
            cameraPos.y -= 10;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            lightPos.z += 10;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            lightPos.z -= 10;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            pos2zoffset += 10;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
        {
            pos2zoffset -= 10;
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mouseReal = Isometric::ScreenposToRealpos({ (float)mousePos.x, (float)mousePos.y }, cameraPos, 
        sf::Vector2u(800, 600));
        
        position2 = {mouseReal.x, mouseReal.y, pos2zoffset};
        //lightPos = sf::Vector3((float)mouseReal.x, (float)mouseReal.y, pos2zoffset);
        //std::cout << lightPos.x << " " << lightPos.y << std::endl;

        isometricDrawer.Render(cameraPos, lightPos);

        guiDrawer.Render();

        // draw image to window
        window.clear();
        glDisable(GL_DEPTH_TEST);
        glUseProgram(0);
        sf::Sprite finalSprite(isometricDrawer.Texture());
        window.draw(finalSprite);
        sf::Sprite guiSprite(guiDrawer.Texture());
        window.draw(guiSprite);
        window.display();
    }

    return 0;
}