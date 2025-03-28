#include "arcpch.h"
#include "arc.h"
#include "functions.h"

#include "fileio/foldersystem.h"
#include "fileio/filesystem.h"
#include "settings/settings.h"
#include "strings/strings.h"
#include "console/console.h"
#include "array/array.h"

namespace functions
{
    void AOs1000()
    {
        console::print("AOs1000!", console::color::LIGHT_WHITE);
        console::print("CONGRATULATIONS! For hitting 1000 LINES OF CODE in AOs 1.3!", console::color::LIGHT_WHITE);
        console::print("It was my first ever program to reach these many LINES OF CODE!", console::color::LIGHT_WHITE);
    }

    void BadApple()
    {
        system("start https://youtu.be/2Ni13dnAbSA");
    }

    void Rockstar6()
    {
        system("start https://youtu.be/QdBZY2fkU-0");
    }

    void itanimulli()
    {
        console::print("spelling mistake kid. It's illuminati", console::color::LIGHT_WHITE);
    }

    void illuminati()
    {
        system("start https://youtu.be/DCqty4-VjZc");
    }

    void wgpcgr()
    {
        console::print("true", console::get_console_back_color(), true);
    }

    void mrstark()
    {
        console::print("And IAmBatMan.", console::get_console_back_color(), true);
    }

    void itsmagic()
    {
        system("start https://youtu.be/dQw4w9WgXcQ");
    }

    void diagxt()
    {
        std::vector<std::string> details = {
            "NAME        : arc ",
            "VERSION     : 1",
            "PROCESS     : arc.exe",
            "SYSTEM TYPE : x64",
            "",
            "AUTHOR           : SrijanSriv211 (Srijan Srivastava)",
            "REGISTERED OWNER : " + std::string(std::getenv("username")),
            "",
            "ROOT DIRECTORY     : " + arc::get_root_path(),
            "SETTINGS FILE      : " + arc::env_path + "\\.arc\\settings.json",
            "",
            "GITHUB REPO     : https://github.com/SrijanSriv211/arc",
            "SYSTEM LANGUAGE : en-in; English (India)"
        };

        console::print(strings::join("\n", details), console::color::LIGHT_WHITE);
    }

    void get_help()
    {
        console::print("The ", console::color::WHITE, false);
        console::print("help ", console::color::GRAY, false);
        console::print("command will give you information about any command that is indexed by arc.", console::color::WHITE);
        console::print("So, to know which commands are indexed and which are not and to change that setting you can open ", console::color::WHITE, false);
        console::print("`.arc/settings.json`.", console::color::GRAY);
        console::print("And furthermore you can visit ", console::color::WHITE, false);
        console::print("`https://github.com/SrijanSriv211/arc` ", console::color::GRAY, false);
        console::print("to get more information about arc ", console::color::WHITE, false);
        console::print("`settings.json`.", console::color::GRAY);
        std::cout << std::endl;

        // all keyboard shortcuts in arc
        std::vector<std::string> shortcut_keys_heading = {
            "| Shortcut              | Comment                         |",
            "| --------------------- | --------------------------------|"
        };

        std::vector<std::string> shortcut_keys = {
            "| `Tab`                 | Next autocomplete suggestions   |",
            "| `Backspace`           | Delete previous character       |",
            "| `UpArrow`             | Previous input in history       |",
            "| `DownArrow`           | Next input in history           |",
            "| `LeftArrow`           | Backward one character          |",
            "| `RightArrow`          | Forward one character           |",
            "| `Ctrl`+`Enter`        | Select current suggestion       |",
            "| `Ctrl`+`Backspace`    | Delete previous word            |",
            "| `Ctrl`+`LeftArrow`    | Backward one word               |",
            "| `Ctrl`+`RightArrow`   | Forward one word                |",
            "| `Ctrl`+`UpArrow`      | First input in history          |",
            "| `Ctrl`+`DownArrow`    | Last input in history           |"
        };

        console::print("All the supported shortcut keys are the following:", console::color::LIGHT_YELLOW);
        console::print(strings::join("\n", shortcut_keys_heading), console::color::LIGHT_WHITE);
        console::print(strings::join("\n", shortcut_keys), console::color::WHITE);
        std::cout << std::endl;

        // all commands in arc
        const json model_access = settings::load()["model_access"];
        std::vector<std::string> help_map_heading = {"envname:", "localhost:", "model_access:", "startlist:", "int. cmds:", "ext. cmds:"};
        std::vector<std::string> help_map = {
            settings::load()["envname"].get<std::string>(),
            std::to_string(settings::load()["localhost"].get<int>()),
            model_access[0].get<std::string>(),
            strings::join(", ", settings::load()["startlist"]),
            strings::join("\n", functions::get_all_cmds(true)),
            strings::join("\n", settings::get_all_cmds())
        };

        for (std::vector<std::string>::size_type i = 0; i < help_map.size(); i++)
        {
            if (strings::is_empty(help_map[i]))
                continue;
            
            console::print(help_map_heading[i], console::LIGHT_YELLOW);
            console::print(help_map[i], console::WHITE);
            std::cout << std::endl;
        }
    }

    // initialize .arc folder with it's subdirectories
    // this folder will contain all the settings for arc
    void init_folders()
    {
        foldersystem::create(".arc");
        filesystem::write(".arc\\settings.json", settings::format);
    }

    void getdir()
    {
        console::print("\n" + std::filesystem::current_path().string(), console::color::LIGHT_WHITE);
    }

    void prevdir()
    {
        foldersystem::change("..");
    }

    // if the string starts and ends with any of these (", ', `), then crop these from the string
    std::string unstring(const std::string& str)
    {
        if (!(strings::startswith_any(str, {"\"", "'", "`"}) & strings::endswith_any(str, {"\"", "'", "`"})))
            return str;
        return str.substr(1, str.size() - 2);
    }

    void chdir(const std::vector<std::string>& path)
    {
        std::filesystem::current_path(strings::trim(unstring(strings::join(" ", path))));
    }

    void chenv(const std::vector<std::string>& path)
    {
        if (strings::trim(unstring(strings::join(" ", path))) == "root")
        {
            arc::env_path = arc::get_root_path();
            return;
        }

        std::string env_path = std::filesystem::current_path().string();
        if (!array::is_empty(path))
        {
            std::string orig_path = std::filesystem::current_path().string();
            chdir(path);
            env_path = std::filesystem::current_path().string();
            chdir({orig_path});
        }

        arc::env_path = env_path;
    }

    void cmd(const std::vector<std::string>& args)
    {
        std::system(strings::trim(strings::join(" ", args)).c_str());
    }

    void touch(const std::vector<std::string>& arr)
    {
        std::string name = strings::trim(unstring(strings::join(" ", arr)));
        if (name.ends_with("/") or name.ends_with("\\"))
        {
            foldersystem::create(name);
            return;
        }
        filesystem::create(name);
    }

    void del(const std::vector<std::string>& arr)
    {
        std::string name = strings::trim(unstring(strings::join(" ", arr)));
        if (name.ends_with("/") or name.ends_with("\\"))
        {
            foldersystem::del(name);
            return;
        }
        filesystem::del(name);
    }

    // { [...] : [...(), true/false] }; if true then "continue" the loop else don't
    std::map<std::vector<std::string>, std::pair<std::function<void()>, bool>> cmd_func_map = {
        // main commands
        {{"diagxt"}, {diagxt, false}},
        {{"cls", "clear",}, {arc::clear_console, true}},
        {{"cd..", "prevdir", "<<"}, {prevdir, false}},
        {{"cd.", "getdir", ".."}, {getdir, true}},
        {{"init", "-i", ":"}, {init_folders, true}},
        {{"help", "/?"}, {get_help, true}},

        // easter egg commands
        {{"_AOs1000"}, {AOs1000, false}},
        {{"_BadApple"}, {BadApple, true}},
        {{"_R*6"}, {Rockstar6, true}},
        {{"_itanimulli"}, {itanimulli, false}},
        {{"_illuminati"}, {illuminati, true}},
        {{"_withgreatpowercomesgreatresponsibility", "--wgpcgr"}, {wgpcgr, false}},
        {{"_IAmIronMan", "--mrstark"}, {mrstark, false}},
        {{"_itsmagicitsmagic"}, {itsmagic, true}}
    };

    // { [...] : [...(...), true/false] }
    std::map<std::vector<std::string>, std::pair<std::function<void(const std::vector<std::string>&)>, bool>> cmd_args_func_map = {
        {{"env", "chenv", "..."}, {chenv, true}},
        {{"touch", "create", "tch", "crt", "|"}, {touch, true}},
        {{"del", "rm", "delete", "remove", "||"}, {del, true}},
        {{"cd", "chdir", ">>"}, {chdir, false}},
        {{"start", "call", ">"}, {cmd, false}},
    };

    std::pair<std::function<void()>, bool> get_cmd_func(const std::string& cmd)
    {
        for (const auto& [key, pair] : cmd_func_map)
        {
            if (strings::any(cmd, key, true))
                return pair;
        }

        return {nullptr, true};
    }

    std::pair<std::function<void(const std::vector<std::string>&)>, bool> get_cmd_args_func(const std::string& cmd)
    {
        for (const auto& [key, pair] : cmd_args_func_map)
        {
            if (strings::any(cmd, key, true))
                return pair;
        }

        return {nullptr, true};
    }

    // rm_egc -> remove easter egg commands
    // commands starting with `_` are easter egg commands
    std::vector<std::string> get_all_cmds(const bool& rm_egc)
    {
        std::vector<std::string> all_cmds = {"exit, quit, ;", "server, -s"};
        for (const auto& [key, pair] : cmd_func_map)
        {
            std::string cmds = strings::join(", ", key);
            if (rm_egc && cmds.starts_with('_'))
                continue;

            all_cmds.push_back(cmds);
        }

        for (const auto& [key, pair] : cmd_args_func_map)
        {
            std::string cmds = strings::join(", ", key);
            if (rm_egc && cmds.starts_with('_'))
                continue;

            all_cmds.push_back(cmds);
        }

        return all_cmds;
    }

    // rm_egc -> remove easter egg commands
    // commands starting with `_` are easter egg commands
    std::vector<std::vector<std::string>> get_all_cmds_list(const bool& rm_egc)
    {
        std::vector<std::vector<std::string>> all_cmds = {{"exit", "quit", ";"}, {"server", "-s"}};
        for (const auto& [key, pair] : cmd_func_map)
        {
            std::string cmds = strings::join(", ", key);
            if (rm_egc && cmds.starts_with('_'))
                continue;

            all_cmds.push_back(key);
        }

        for (const auto& [key, pair] : cmd_args_func_map)
        {
            std::string cmds = strings::join(", ", key);
            if (rm_egc && cmds.starts_with('_'))
                continue;

            all_cmds.push_back(key);
        }

        return all_cmds;
    }
}
