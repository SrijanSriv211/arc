#include "arcpch.h"
#include "settings.h"
#include "arc.h"

#include "strings/strings.h"
#include "console/console.h"
#include "array/array.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace settings
{
    std::string format = R"([
    {
        "names": [],
        "paths": [],
        "argid": -1
    }
]
)";

    json load()
    {
        std::string env_path = arc::arc_env_path + "\\.arc\\settings.json";
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

    int get_command_by_name(const std::string& cmd)
    {
        json commands = load();

        for (size_t i = 0; i < commands.size(); i++)
        {
            if (array::is_empty(array::trim(commands[i]["names"])))
                continue;

            if (strings::any(cmd, array::trim(commands[i]["names"])))
                return i;
        }

        return -1;
    }

    void run_command_by_id(const std::string& cmd, const std::vector<std::string>& args)
    {
        int id = get_command_by_name(cmd);
        json command = load()[id];

        int arg_idx = std::stoi(command["argid"].dump());

        if (arg_idx >= static_cast<int>(command["paths"].size()) || arg_idx < -1)
        {
            console::errors::runtime("argid = " + command["argid"].dump(), "Invalid setting for `argid` at command index " + std::to_string(id));
            return;
        }

        std::vector<std::string> paths = command["paths"];

        if (arg_idx >= 0)
        {
            paths[arg_idx] += " " + strings::join("", args);
            paths[arg_idx] = strings::trim(paths[arg_idx]);
        }

        std::system(strings::join(" && ", paths).c_str());
    }
}
