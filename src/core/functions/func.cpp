#include "arcpch.h"
#include "arc.h"

#include "fileio/foldersystem.h"
#include "fileio/filesystem.h"
#include "settings/settings.h"
#include "strings/strings.h"
#include "console/console.h"
#include "stylus/stylus.h"
#include "array/array.h"

namespace arc
{
    int is_running()
    {
        while(true)
        {
            arc::print_prompt();

            console::stylus s = console::stylus();
            std::vector<std::string> tokens = s.read();

            if (tokens.empty())
                continue;

            // break tokens into `cmd` and `args`
            std::string cmd = tokens.front();
            std::vector<std::string> args = array::trim(std::vector<std::string>(tokens.begin() + 1, tokens.end()));

            if (strings::any(cmd, {"exit", "quit", ";"}, true))
                return 0;

            else if (strings::any(cmd, {"cls", "clear"}, true))
            {
                arc::clear_console();
                continue;
            }

            else if (strings::any(cmd, {"cd.", "getdir", ".."}, true))
                console::print("\n" + std::filesystem::current_path().string(), console::color::LIGHT_WHITE);

            else if (strings::any(cmd, {"cd..", "cmbk", "<<"}, true))
                foldersystem::change("..");

            else if (strings::any(cmd, {"cd", "chdir", ">>"}, true))
            {
                if (args.empty())
                    console::errors::runtime(cmd, "No folder path found");

                foldersystem::change(args.front());
            }

            else if (strings::any(cmd, {"init", "-i"}, true))
            {
                // initialize .arc folder with it's subdirectories
                // this folder will contain all the settings for arc
                foldersystem::create(".arc");
                foldersystem::create(".arc\\etc");
                filesystem::write(".arc\\settings.json", settings::format);
                continue;
            }

            else if (settings::get_command_by_name(cmd) != -1)
                settings::run_command_by_id(cmd, args);

            else
                console::errors::runtime(cmd, "Command not found");

            std::cout << std::endl;
        }

        return 0;
    }
}
