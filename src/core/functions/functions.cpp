#include "arcpch.h"
#include "arc.h"
#include "functions.h"

#include "fileio/foldersystem.h"
#include "fileio/filesystem.h"
#include "settings/settings.h"
#include "strings/strings.h"
#include "console/console.h"

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
            "SETTINGS FILE      : " + arc::arc_env_path + "\\.ao\\settings.json",
            "",
            "GITHUB REPO     : https://github.com/SrijanSriv211/arc",
            "SYSTEM LANGUAGE : en-in; English (India)"
        };

        console::print(strings::join("\n", details), console::color::LIGHT_WHITE);
    }

    // initialize .arc folder with it's subdirectories
    // this folder will contain all the settings for arc
    void init_folders()
    {
        foldersystem::create(".arc");
        foldersystem::create(".arc\\etc");
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

    void chdir(const std::vector<std::string>& path)
    {
        std::filesystem::current_path(path.front());
    }

    void cmd(const std::vector<std::string>& args)
    {
        std::system(strings::trim(strings::join("", args)).c_str());
    }

    // { [...] : [...(), true/false] }; if true then "continue" the loop else don't
    std::map<std::vector<std::string>, std::pair<std::function<void()>, bool>> cmd_func_map = {
        // main commands
        {{"cls", "clear",}, {arc::clear_console, true}},
        {{"cd..", "prevdir", "<<"}, {prevdir, false}},
        {{"cd.", "getdir", ".."}, {getdir, false}},
        {{"init", "-i"}, {init_folders, true}},

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
}
