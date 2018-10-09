#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
//#include <GL/glext.h>
#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <experimental/filesystem>
#include <unordered_map>
#include <optional>
#include "tinyexr.hpp"

class Object
{
public:
    Object(sf::Texture const& colorMap, sf::Texture const& normalMap) :
        colorMap(colorMap),
        normalMap(normalMap)
    {}

    sf::Texture const& colorMap;
    sf::Texture const& normalMap;
    sf::Vector2f position;
    sf::Vector2f origin;

    sf::Vector2f RealposToScreenpos(sf::Vector2f const& cameraPos)
    {
        sf::Transform fromIsoToScreen;
        fromIsoToScreen.rotate(45);
        fromIsoToScreen.scale(1.0f, -0.5f);
        return fromIsoToScreen.transformPoint(position);
    }

    void Draw(sf::RenderTarget& renderTarget, sf::Vector2f const& cameraPos)
    {
        sf::Sprite sprite;
        sprite.setPosition(RealposToScreenpos(position));
        sprite.setTexture(colorMap);
        renderTarget.draw(sprite);
    }
};

class TextureLoader
{
    std::experimental::filesystem::path dir;
    std::unordered_map<std::string, sf::Texture> textures;
public:
    TextureLoader(std::experimental::filesystem::path dir) : dir(std::move(dir)) {}
    std::optional<sf::Texture const*> Get(std::string const& tex)
    {
        auto res = textures.try_emplace(tex);
        if (res.second)
        {
            if (!res.first->second.loadFromFile((dir / tex).string()))
            {
                return std::nullopt;
            }
        }
        
        return { &res.first->second };
    }

    std::optional<GLuint> GetDepthMap(std::string const& map)
    {
        float *rgba = nullptr;
        int width, height;
        const char *err = nullptr;
        int ret = LoadEXR(&rgba, &width, &height, (dir / map).string().c_str(), &err);
        if (TINYEXR_SUCCESS != ret)
        {
            std::cerr << "Failed to load EXR file [" << map << "] code = " << ret << std::endl;
            return std::nullopt;
        }

        std::vector<std::uint8_t> datau8;
        datau8.resize(size_t(width * height * 4));
        for (size_t y = 0; y < (size_t)height; y++)
        {
            for (size_t x = 0; x < (size_t)width; x++)
            {
                datau8[(y*width+x) * 4 + 0] = (std::uint8_t)((rgba[4 * (y*width+x) + 0]-48)/3.0f*255.0f);
                datau8[(y*width+x) * 4 + 1] = (std::uint8_t)((rgba[4 * (y*width+x) + 0]-48)/3.0f*255.0f);
                datau8[(y*width+x) * 4 + 2] = (std::uint8_t)((rgba[4 * (y*width+x) + 0]-48)/3.0f*255.0f);
                datau8[(y*width+x) * 4 + 3] = 255;
                //std::cout << (std::uint32_t)datau8[(y*width+x) * 4 + 0] << std::endl;
            }
        }

        GLuint texid;
        glGenTextures(1, &texid);
        glBindTexture(GL_TEXTURE_2D, texid);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, datau8.data());
        //glGenerateMipmap(GL_TEXTURE_2D);

        return { texid };
    }
};

std::optional<Object> CreateCube(TextureLoader& texLoader)
{
    auto texColor = texLoader.Get("cube.color.png");
    if (!texColor)
        return std::nullopt;
    auto texNormal = texLoader.Get("cube.normal.png");
    if (!texNormal)
        return std::nullopt;

    return Object(**texColor, **texNormal);
}

uint64_t unix_timestamp()
{
    std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch()
    );
    return ms.count();
}

int main()
{
    TextureLoader texLoader(std::experimental::filesystem::path("./tex"));

    sf::Vector2f cameraPos(0,0);

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "glew init error" << std::endl;
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return -1;
    }

    auto cube1 = CreateCube(texLoader);
    if (!cube1)
        return -1;

    /*auto cube2 = CreateCube(texLoader);
    if (!cube2)
        return -1;
    cube2->position = {-100, 100};*/

    auto depthmap = texLoader.GetDepthMap("cube.depth.exr");
    if (!depthmap)
        return -1;

    sf::Shader lighting;
    if (!lighting.loadFromFile("lighting.frag", sf::Shader::Fragment))
    {
        return -1;
    }
    
    GLuint colorIndex = glGetUniformLocation(lighting.getNativeHandle(), "colormap");
    if (colorIndex == -1)
    {
        std::cout << "Could not find depth!" << std::endl;
        return -1;
    }
    GLuint normalIndex = glGetUniformLocation(lighting.getNativeHandle(), "normalmap");
    if (normalIndex == -1)
    {
        std::cout << "Could not find depth!" << std::endl;
        return -1;
    }
    GLuint depthIndex = glGetUniformLocation(lighting.getNativeHandle(), "depthmap");
    if (depthIndex == -1)
    {
        std::cout << "Could not find depth!" << std::endl;
        return -1;
    }

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

        // clear the window with black color
        window.clear(sf::Color::Blue);

        std::uint64_t unixtime = unix_timestamp();
        std::uint64_t botpart = unixtime;
        botpart /= 1000000;
        unixtime -= botpart * 1000000;
        float movespeed = 0.002f;
        //sf::Vector3f lightpos(5.0f*(float)std::cos(((float)unixtime)*movespeed),1.0f,5.0f*(float)std::sin(((float)unixtime)*movespeed));
        sf::Vector3f lightpos(5+10.0f*(float)std::cos(((float)unixtime)*movespeed), 5, 5);
        lighting.setUniform("lightpos", lightpos);
        lighting.setUniform("normalmap", cube1->normalMap);

        sf::Shader::bind(&lighting);
        //cube1->position += sf::Vector2f(0, movespeed);
        glUniform1i(depthIndex, 2);
        glActiveTexture(GL_TEXTURE0 + 2);
        glBindTexture(GL_TEXTURE_2D, *depthmap);
        glActiveTexture(GL_TEXTURE0);

        //glUniform1i(colorIndex, GL_TEXTURE0);
        //glUniform1i(normalIndex, GL_TEXTURE1);
        //glUniform1i(depthIndex, GL_TEXTURE31);
       
        //glActiveTexture(GL_TEXTURE2);
        //glBindTexture(GL_TEXTURE_2D, cube1->normalMap.getNativeHandle());
        //glActiveTexture(GL_TEXTURE0);
       
        cube1->Draw(window, cameraPos);
        //cube2->Draw(lighting, window, cameraPos);

        // end the current frame
        window.display();
    }

    return 0;
}