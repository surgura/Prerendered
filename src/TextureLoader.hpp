#pragma once

#include <cstdint>
#include <string>
#include <experimental/filesystem>
#include <unordered_map>
#include <optional>
#include <SFML/Graphics/Texture.hpp>

class TextureLoader
{
    std::experimental::filesystem::path directory;
public:
    TextureLoader(std::experimental::filesystem::path directory);
    std::optional<sf::Texture> GetColorMap(std::string const& name);
    std::optional<std::uint32_t> GetNormalMap(std::string const& name);
    std::optional<std::uint32_t> GetPositionMap(std::string const& name);
};