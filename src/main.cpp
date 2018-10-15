#include <GL/glew.h>
#include <iostream>
#include <optional>
#include "TextureLoader.hpp"
#include "IsometricDrawer.hpp"
#include "Isometric.hpp"

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
    sf::Vector3f lightPos(0,0,0);

    IsometricDrawer isometricDrawer;

    auto drawable1 = CreateCubeDrawable(texLoader);
    if (!drawable1)
        return -1;
    sf::Vector2f position1;

    auto drawable2 = CreateCubeDrawable(texLoader);
    if (!drawable2)
        return -1;
    sf::Vector2f position2;

    isometricDrawer.Add(position1, *drawable1);
    //isometricDrawer.Add(position2, *drawable2);

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

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mouseReal = Isometric::ScreenposToRealpos({ (float)mousePos.x, (float)mousePos.y }, cameraPos, 
        sf::Vector2u(800, 600));
        
        position2 = mouseReal;
        lightPos = sf::Vector3((float)mouseReal.x, (float)mouseReal.y, 100.0f);
        position2 = mouseReal;
        std::cout << lightPos.x << " " << lightPos.y << std::endl;

        isometricDrawer.Render(cameraPos, lightPos);

        // draw image to window
        window.clear();
        glDisable(GL_DEPTH_TEST);
        glUseProgram(0);
        sf::Sprite finalSprite(isometricDrawer.Texture());
        window.draw(finalSprite);
        window.display();
    }

    return 0;
}