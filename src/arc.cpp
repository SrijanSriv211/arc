#include "arcpch.h"
#include "arc.h"

#include "fileio/foldersystem.h"
#include "console/console.h"
#include "datetime/datetime.h"
#include "strings/strings.h"
#include "token/token.h"

namespace arc
{
    std::string current_path;

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

        current_path = std::filesystem::current_path().string();
        console::print(std::filesystem::current_path().string(), console::color::LIGHT_WHITE);
    }

    void print_prompt()
    {
        if (current_path != std::filesystem::current_path().string())
        {
            current_path = std::filesystem::current_path().string();
            console::print(std::filesystem::current_path().string(), console::color::LIGHT_WHITE);
        }

        console::print(">~ ", console::color::GRAY, false);
    }

    int is_running()
    {
        std::string input;

        while(true)
        {
            arc::print_prompt();

            // IO error!
            if (!std::getline(std::cin, input))
                return -1;

            lex::tokenize(input);

            if (input.empty())
                continue;

            else if (strings::any(input, {"exit", "quit", ";"}, true))
                return 0;

            else if (strings::any(input, {"cls", "clear"}, true))
            {
                arc::clear_console();
                continue;
            }

            else if (strings::any(input, {"cd", "chdir", ">>"}, true))
                foldersystem::change("scripts");

            std::cout << std::endl;
        }

        return 0;
    }
}
