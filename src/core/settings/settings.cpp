#include "arcpch.h"
#include "settings.h"
#include "arc.h"

#include "fileio/filesystem.h"
#include "nlohmann/json.hpp"
#include "strings/strings.h"
#include "console/console.h"
#include "array/array.h"

using json = nlohmann::json;

namespace settings
{
    std::string format = R"({
    "envname": "root",
    "localhost": 8000,
    "model_access": ["deepseek-r1-distill-llama-70b", "groq-api-key"],
    "startlist": [],
    "cmd": [
        {
            "names": [],
            "paths": [],
            "id": -1,
            "noline": false
        }
    ]
}
)";

    json load()
    {
        std::string env_path = arc::env_path + "\\.arc\\settings.json";
        if (!std::filesystem::exists(env_path))
        {
            env_path = arc::get_root_path() + "\\settings.json";
            if (!std::filesystem::exists(env_path))
                return json::parse(format);

            std::ifstream f(env_path);
            return json::parse(f);
        }

        std::ifstream f(env_path);
        return json::parse(f);
    }

    std::vector<std::string> get_all_cmds()
    {
        std::vector<std::string> all_cmds = {};
        json commands = load()["cmd"];

        for (size_t i = 0; i < commands.size(); i++)
        {
            if (array::is_empty(array::trim(commands[i]["names"])))
                continue;

            std::string names = strings::join(", ", array::trim(commands[i]["names"]));
            all_cmds.push_back(names);
        }

        return all_cmds;
    }

    std::vector<std::vector<std::string>> get_all_cmds_list()
    {
        std::vector<std::vector<std::string>> all_cmds = {};
        json commands = load()["cmd"];

        for (size_t i = 0; i < commands.size(); i++)
        {
            if (array::is_empty(array::trim(commands[i]["names"])))
                continue;

            all_cmds.push_back(array::trim(commands[i]["names"]));
        }

        return all_cmds;
    }

    int get_command_by_name(const std::string& cmd)
    {
        json commands = load()["cmd"];

        for (size_t i = 0; i < commands.size(); i++)
        {
            if (array::is_empty(array::trim(commands[i]["names"])))
                continue;

            if (strings::any(cmd, array::trim(commands[i]["names"])))
                return i;
        }

        return -1;
    }

    bool run_command_by_id(const std::string& cmd, const std::vector<std::string>& args)
    {
        int id = get_command_by_name(cmd);
        json command = load()["cmd"][id];

        int arg_idx = std::stoi(command["id"].dump());

        if (arg_idx >= static_cast<int>(command["paths"].size()) || arg_idx < -1)
        {
            console::errors::runtime("id = " + command["id"].dump(), "Invalid setting for `id` at command index " + std::to_string(id));
            std::cout << std::endl;
            return false;
        }

        std::vector<std::string> paths = command["paths"];

        if (arg_idx >= 0)
        {
            paths[arg_idx] += " " + strings::join("", args);
            paths[arg_idx] = strings::trim(paths[arg_idx]);
        }

        std::system(strings::join(" && ", paths).c_str());
        return command["noline"];
    }
}
