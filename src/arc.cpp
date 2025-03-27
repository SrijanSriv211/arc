#include "arcpch.h"
#include "arc.h"

#include "functions/functions.h"
#include "settings/settings.h"
#include "fileio/filesystem.h"
#include "datetime/datetime.h"
#include "strings/strings.h"
#include "console/console.h"
#include "stylus/stylus.h"
#include "server/server.h"
#include "array/array.h"
#include "llm/llm.h"

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

    // =====================================================================
    // ============================= Exec Code =============================
    // =====================================================================

    int exec_tokens(const std::vector<std::string>& tokens)
    {
        if (array::is_empty(tokens))
            return 1;

        // break tokens into `cmd` and `args`
        std::string cmd = tokens.front();
        std::vector<std::string> args = array::trim(std::vector<std::string>(tokens.begin() + 1, tokens.end()));

        if (strings::any(cmd, {"exit", "quit", ";"}, true))
            return 0;

        else if (strings::any(cmd, {"server", "-s"}, true))
            return -1;

        else if (functions::get_cmd_func(cmd).first != nullptr)
        {
            std::pair<std::function<void()>, bool> func_map = functions::get_cmd_func(cmd);
            (func_map.first)();
            if (func_map.second == true)
                return 1;
        }

        else if (functions::get_cmd_args_func(cmd).first != nullptr)
        {
            std::pair<std::function<void(const std::vector<std::string>&)>, bool> func_map = functions::get_cmd_args_func(cmd);
            (func_map.first)(args);
            if (func_map.second == true)
                return 1;
        }

        else if (settings::get_command_by_name(cmd) != -1)
        {
            if (settings::run_command_by_id(cmd, args))
                return 1;
        }

        else
        {
            std::string MODEL = settings::load()["model_access"][0];
            std::string API_KEY = settings::load()["model_access"][1];

            if (strings::is_empty(API_KEY) || API_KEY == "groq-api-key" || API_KEY == "api-key" || API_KEY == "GROQ-API-KEY" || API_KEY == "API-KEY")
            {
                console::errors::runtime(cmd, "Command not found\n");
                return 1;
            }

            llm::generate(strings::join("", tokens), MODEL, API_KEY);
        }

        std::cout << std::endl;
        return 1;
    }

    int is_running()
    {
        UINT code_page = GetConsoleOutputCP();

        // change code page to UTF-8
        if (code_page != 65001)
        {
            // https://stackoverflow.com/a/388500/18121288
            // https://stackoverflow.com/questions/45575863/how-to-print-utf-8-strings-to-stdcout-on-windows
            SetConsoleOutputCP(65001);
        }

        console::stylus s = console::stylus();
        int return_code = 1;
        while(return_code)
        {
            arc::print_prompt();
            std::vector<std::string> tokens = s.read();
            return_code = arc::exec_tokens(tokens);
            if (return_code == 0 || return_code == -1)
                break;
        }

        // curl -X POST http://127.0.0.1:8000 -d "shout Hello world!"
        if (return_code == -1)
            start_server("127.0.0.1", settings::load()["localhost"]);

        SetConsoleCP(code_page); // reset code page to the original one.
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
