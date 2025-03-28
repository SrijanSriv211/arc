#pragma once

namespace filesystem
{
    void create(const std::filesystem::path& filepath);
    void write(const std::filesystem::path& filepath, const std::string& content);
    void del(const std::filesystem::path& filepath);
}
