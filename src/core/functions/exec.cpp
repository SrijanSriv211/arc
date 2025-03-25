#include "arcpch.h"
#include "arc.h"

#include "functions/functions.h"
#include "fileio/foldersystem.h"
#include "fileio/filesystem.h"
#include "settings/settings.h"
#include "strings/strings.h"
#include "console/console.h"
#include "stylus/stylus.h"
#include "array/array.h"
#include "llm/llm.h"

namespace arc
{
    int is_running()
    {
        console::stylus s = console::stylus();
        while(true)
        {
            arc::print_prompt();
            std::vector<std::string> tokens = s.read();

            if (array::is_empty(tokens))
                continue;

            // break tokens into `cmd` and `args`
            std::string cmd = tokens.front();
            std::vector<std::string> args = array::trim(std::vector<std::string>(tokens.begin() + 1, tokens.end()));

            if (strings::any(cmd, {"exit", "quit", ";"}, true))
                return 0;

            else if (strings::any(cmd, {"help", "/?"}, true))
            {
                const json model_access = settings::load()["model_access"];
                std::map<std::string, std::string> help_map = {
                    {"envname:", settings::load()["envname"].get<std::string>()},
                    {"model_access:", model_access[0].get<std::string>()},
                    {"startlist:", strings::join(", ", settings::load()["startlist"])},
                    {"int. cmds:", strings::join("\n", functions::get_all_cmds())},
                    {"ext. cmds:", strings::join("\n", settings::get_all_cmds())}
                };

                bool is_first_item = true;
                std::vector<std::string> helps = {};
                for (const auto& [key, pair] : help_map)
                {
                    if (strings::is_empty(pair))
                        continue;

                    if (!is_first_item)
                        std::cout << std::endl;

                    console::print(key, console::LIGHT_WHITE);
                    std::cout << pair << std::endl;
                    is_first_item = false;
                }
            }

            else if (functions::get_cmd_func(cmd).first != nullptr)
            {
                std::pair<std::function<void()>, bool> func_map = functions::get_cmd_func(cmd);
                (func_map.first)();
                if (func_map.second == true)
                    continue;
            }

            else if (functions::get_cmd_args_func(cmd).first != nullptr)
            {
                std::pair<std::function<void(const std::vector<std::string>&)>, bool> func_map = functions::get_cmd_args_func(cmd);
                (func_map.first)(args);
                if (func_map.second == true)
                    continue;
            }

            else if (settings::get_command_by_name(cmd) != -1)
            {
                if (settings::run_command_by_id(cmd, args))
                    continue;
            }

            else
            {
                std::string MODEL = settings::load()["model_access"][0];
                std::string API_KEY = settings::load()["model_access"][1];

                if (strings::is_empty(API_KEY) || API_KEY == "groq-api-key" || API_KEY == "api-key" || API_KEY == "GROQ-API-KEY" || API_KEY == "API-KEY")
                {
                    console::errors::runtime(cmd, "Command not found\n");
                    continue;
                }

                llm::generate(strings::join("", tokens), MODEL, API_KEY);
            }

            std::cout << std::endl;
        }

        return 0;
    }

    void run_startlist()
    {
        std::vector<std::string> startlist = settings::load()["startlist"];
        if (array::is_empty(startlist))
            return;

        for (const std::string& filename : startlist)
        {
            console::print("> ", console::color::GRAY, false);
            console::print(filename, console::color::LIGHT_WHITE);
            std::system(filename.c_str());
            std::cout << std::endl;
        }
    }
}
