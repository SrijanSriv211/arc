#pragma once

namespace arc
{
    extern std::string arc_env_path;

    std::string get_root_path();
    void clear_console();
    void print_prompt();
    int is_running();
}
