#pragma once

namespace foldersystem
{
    void create(const std::filesystem::path& folderpath);
    void change(const std::filesystem::path& folderpath);
    void del(const std::filesystem::path& folderpath);
}
