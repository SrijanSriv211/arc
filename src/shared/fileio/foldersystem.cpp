#include "arcpch.h"
#include "foldersystem.h"
#include "console/console.h"

namespace foldersystem
{
    void create(const std::filesystem::path& folderpath)
    {
        // check if the folder already exists
        if (std::filesystem::exists(folderpath))
            return;

        // create the folder
        try
        {
            if (!std::filesystem::create_directory(folderpath))
                console::errors::throw_error("Failed to create folder.", "Folder system IO");
        }

        catch(const std::exception& e)
        {
            console::errors::throw_error(e.what(), "C++ folder system IO");
        }
    }

    void change(const std::filesystem::path& folderpath)
    {
        // check if the folder exists
        if (!std::filesystem::exists(folderpath))
        {
            console::errors::throw_error("Cannot find folder.", "Folder system IO");
            return;
        }

        // change the folder
        try
        {
            std::filesystem::current_path(folderpath);
        }

        catch(const std::exception& e)
        {
            console::errors::throw_error(e.what(), "C++ folder system IO");
        }
    }

    void del(const std::filesystem::path& folderpath)
    {
        // check if the folder exists
        if (!std::filesystem::exists(folderpath))
        {
            console::errors::throw_error("Cannot find folder.", "Folder system IO");
            return;
        }

        // change the folder
        try
        {
            std::filesystem::remove_all(folderpath);
        }

        catch(const std::exception& e)
        {
            console::errors::throw_error(e.what(), "C++ folder system IO");
        }
    }
}
