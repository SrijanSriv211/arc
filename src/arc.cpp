#include "arcpch.h"
#include "arc.h"

#include "fileio/filesystem.h"
#include "datetime/datetime.h"
#include "console/console.h"
#include "settings/settings.h"

namespace arc
{
    std::string current_path;
    std::string env_path = std::filesystem::current_path().string();

    // https://stackoverflow.com/q/50889647/18121288
    std::string get_root_path()
    {
        char buffer[MAX_PATH];
        // in the stackoverflow thread, the code snippet contained `GetModuleFileName` change it to `GetModuleFileNameA`.
        // `GetModuleFileName` is a macro that resolves to either `GetModuleFileNameA` or `GetModuleFileNameW` based on
        // the project's character set setting. `GetModuleFileNameA` is the ANSI version that works with char strings.
        //*NOTE: explanation by chatgpt
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        std::string::size_type pos = std::string(buffer).find_last_of("\\/");
        return (pos == std::string::npos) ? "" : std::string(buffer).substr(0, pos);
    }

    void clear_console()
    {
        system("cls");
        console::print(datetime::datetime("%a, %d %b %Y"), console::color::LIGHT_WHITE, false);
        console::print("  (" + std::string(std::getenv("username")) + ")", console::color::GRAY);

        // ask user to use AO in Windows Terminal if they are not already
        const char* env_var_val = std::getenv("WT_SESSION");
        if (env_var_val == nullptr || env_var_val[0] == '\0')
        {
            console::print("> ", console::color::GRAY, false);
            console::print("Please use AO in Windows Terminal for better experience.", console::color::RED);
        }

        current_path = "";
    }

    void print_prompt()
    {
        if (current_path != std::filesystem::current_path().string())
        {
            current_path = std::filesystem::current_path().string();
            console::print(std::filesystem::current_path().string(), console::color::LIGHT_WHITE);
        }

        console::print(settings::load()["envname"], console::color::LIGHT_RED, false);
        console::print("~> ", console::color::GRAY, false);
    }
}
