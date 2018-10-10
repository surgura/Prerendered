#pragma once

#include <cstdint>
#include <string>
#include <experimental/filesystem>
#include <unordered_map>
#include <optional>

class TextureLoader
{
    std::experimental::filesystem::path directory;
public:
    TextureLoader(std::experimental::filesystem::path directory);
    std::optional<std::uint32_t> GetColorMap(std::string const& name);
    std::optional<std::uint32_t> GetNormalMap(std::string const& name);
    std::optional<std::uint32_t> GetDepthMap(std::string const& name);
};