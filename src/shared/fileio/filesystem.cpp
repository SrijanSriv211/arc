#include "arcpch.h"
#include "filesystem.h"
#include "console/console.h"

namespace filesystem
{
    void create(const std::filesystem::path& filepath)
    {
        // check if the file already exists
        if (std::filesystem::exists(filepath))
            return;

        // create the folder
        try
        {
            std::ofstream file(filepath);
            file.close();
        }

        catch(const std::exception& e)
        {
            console::errors::throw_error(e.what(), "C++ file system IO");
        }
    }

    void write(const std::filesystem::path& filepath, const std::string& content)
    {
        // check if the file already exists
        filesystem::create(filepath);

        // create the folder
        try
        {
            std::ofstream file(filepath);
            file << content;
            file.close();
        }

        catch(const std::exception& e)
        {
            console::errors::throw_error(e.what(), "C++ file system IO");
        }
    }

    std::vector<std::string> readlines(const std::filesystem::path& filepath)
    {
        // check if the file exists
        if (!std::filesystem::exists(filepath))
        {
            console::errors::throw_error("Cannot find file.", "File system IO");
            return {};
        }

        // create the folder
        std::vector<std::string> lines = {};
        try
        {
            std::string line;
            std::fstream file;
            file.open(filepath);

            if (!file)
                console::errors::throw_error("Unable to open file.", "File system IO");

            while (std::getline(file, line))
                lines.push_back(line);
            file.close();
        }

        catch(const std::exception& e)
        {
            console::errors::throw_error(e.what(), "C++ file system IO");
        }
        return lines;
    }

    void del(const std::filesystem::path& filepath)
    {
        // check if the file exists
        if (!std::filesystem::exists(filepath))
        {
            console::errors::throw_error("Cannot find file.", "File system IO");
            return;
        }

        // change the folder
        try
        {
            std::filesystem::remove(filepath);
        }

        catch(const std::exception& e)
        {
            console::errors::throw_error(e.what(), "C++ file system IO");
        }
    }
}
