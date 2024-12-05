#include "arcpch.h"
#include "arc.h"

#include "console/console.h"
#include "datetime/datetime.h"

namespace arc
{
    void clear_console()
    {
        system("cls");
        console::print(datetime::datetime("%a, %d %b %Y"), console::color::LIGHT_WHITE);

        // ask user to use AO in Windows Terminal if they are not already
        const char* env_var_val = std::getenv("WT_SESSION");
        if (env_var_val == nullptr || env_var_val[0] == '\0')
        {
            console::print("> ", console::color::GRAY, false);
            console::print("Please use AO in Windows Terminal for better experience.", console::color::RED);
        }
    }

    void print_prompt()
    {
        console::print(std::filesystem::current_path().string(), console::color::WHITE);
        console::print(std::string(std::getenv("username")) + "~ ", console::color::GRAY, false);
    }
}
