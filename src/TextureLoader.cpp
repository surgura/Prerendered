#include "TextureLoader.hpp"
#include <cstdint>
#include <vector>
#include "lodepng.h"
#include <iostream>
#include <GL/glew.h>
#include "tinyexr.hpp"

TextureLoader::TextureLoader(std::experimental::filesystem::path directory) :
    directory(directory)
{}

std::optional<sf::Texture> TextureLoader::GetColorMap(std::string const& name)
{
    sf::Texture tex;
    if (!tex.loadFromFile((directory / name).string()))
    {
        return std::nullopt;
    }
    return { tex };

    /*
    std::vector<std::uint8_t> data;
    std::uint32_t width, height;
    std::uint32_t error = lodepng::decode(data, width, height, (directory / name).string());
    if (error)
    {
        std::cout << "Error loading PNG " << error << ": " << lodepng_error_text(error) << std::endl;
        return std::nullopt;
    }
    
    GLuint texid;
    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    return { texid };*/
}

std::optional<std::uint32_t> TextureLoader::GetNormalMap(std::string const& name)
{
    float *rgba = nullptr;
    std::int32_t width, height;
    const char *err = nullptr;
    std::int32_t ret = LoadEXR(&rgba, &width, &height, (directory / name).string().c_str(), &err);
    if (TINYEXR_SUCCESS != ret)
    {
        std::cerr << "Failed to load EXR file [" << name << "] code = " << ret << std::endl;
        return std::nullopt;
    }

    std::vector<float> data;
    data.resize(std::size_t(width * height*3));
    for (std::size_t y = 0; y < (std::size_t)height; y++)
    {
        for (std::size_t x = 0; x < (std::size_t)width; x++)
        {
            data[3*(y*width+x)+0] = rgba[4 * (y*width+x)+0];
            data[3*(y*width+x)+1] = rgba[4 * (y*width+x)+1];
            data[3*(y*width+x)+2] = rgba[4 * (y*width+x)+2];
        }
    }

    GLuint texid;
    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    return { texid };
}

std::optional<std::uint32_t> TextureLoader::GetDepthMap(std::string const& name)
{
    float *rgba = nullptr;
    std::int32_t width, height;
    const char *err = nullptr;
    std::int32_t ret = LoadEXR(&rgba, &width, &height, (directory / name).string().c_str(), &err);
    if (TINYEXR_SUCCESS != ret)
    {
        std::cerr << "Failed to load EXR file [" << name << "] code = " << ret << std::endl;
        return std::nullopt;
    }

    std::vector<float> data;
    data.resize(std::size_t(width * height));
    for (std::size_t y = 0; y < (std::size_t)height; y++)
    {
        for (std::size_t x = 0; x < (std::size_t)width; x++)
        {
            data[y*width+x] = rgba[4 * (y*width+x)];
        }
    }

    GLuint texid;
    glGenTextures(1, &texid);
    glBindTexture(GL_TEXTURE_2D, texid);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT, data.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    return { texid };
}