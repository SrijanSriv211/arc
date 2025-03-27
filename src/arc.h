#pragma once

namespace arc
{
    extern std::string env_path;

    std::string get_root_path();
    void clear_console();
    void print_prompt();

    int exec_tokens(const std::vector<std::string>& tokens);
    int is_running();
    void run_startlist();
}
