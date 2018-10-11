#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <unordered_map>
#include <optional>
#include "TextureLoader.hpp"
#include "IsometricDrawer.hpp"
#include "Isometric.hpp"

class Object
{
public:
    Object(sf::Texture colorMap, std::uint32_t normalMap) :
        colorMap(colorMap),
        normalMap(normalMap),
        scale(1,1)
    {}

    sf::Texture colorMap;
    std::uint32_t normalMap;
    sf::Vector2f position;
    sf::Vector2f origin;
    sf::Vector2f scale;

    sf::Vector2f RealposToScreenpos(sf::Vector2f const& cameraPos)
    {
        sf::Transform fromIsoToScreen;
        fromIsoToScreen.rotate(45);
        sf::Vector2f screenPos = fromIsoToScreen.transformPoint(position - sf::Vector2f(-cameraPos.x, cameraPos.y));
        screenPos.y *= 0.5;
        return -screenPos;
    }

    void Draw(sf::RenderTarget& renderTarget, sf::Vector2f const& cameraPos)
    {
        sf::Sprite sprite;
        sprite.setPosition(RealposToScreenpos(cameraPos) + sf::Vector2f((float)renderTarget.getSize().x, (float)renderTarget.getSize().y)/2.0f);
        sprite.setTexture(colorMap);
        sprite.setScale(scale);
        sprite.setOrigin(origin);
        renderTarget.draw(sprite);
    }
};

std::optional<IsometricDrawable> CreateCubeDrawable(TextureLoader& texLoader)
{
    auto colorMap = texLoader.GetColorMap("cube.color.png");
    if (!colorMap)
        return std::nullopt;
    auto normalMap = texLoader.GetNormalMap("cube.normal.png");
    if (!normalMap)
        return std::nullopt;
    auto depthMap = texLoader.GetDepthMap("cube.depth.exr");
    if (!depthMap)
        return std::nullopt;

    IsometricDrawable obj(*colorMap, *normalMap, *depthMap);
    obj.origin = { 242, 538 };

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

    IsometricDrawer isometricDrawer;

    auto drawableCube1 = CreateCubeDrawable(texLoader);
    if (!drawableCube1)
        return -1;
    sf::Vector2f positionCube1;

    auto drawableCube2 = CreateCubeDrawable(texLoader);
    if (!drawableCube2)
        return -1;
    sf::Vector2f positionCube2;

    isometricDrawer.Add(positionCube1, *drawableCube1);
    isometricDrawer.Add(positionCube2, *drawableCube2);

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
        sf::Vector2f mouseReal = Isometric::ScreenposToRealpos({ (float)mousePos.x, (float)mousePos.y }, cameraPos, sf::Vector2u(800, 600));
        positionCube2 = mouseReal;

        isometricDrawer.Render(cameraPos);

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