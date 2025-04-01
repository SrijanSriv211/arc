#pragma once

#include "nlohmann/json.hpp"
using json = nlohmann::json;

namespace settings
{
    extern std::string format;
    json load();
    std::vector<std::string> load_history();
    void save_history(const std::vector<std::string>& history);
    std::vector<std::string> get_all_cmds();
    std::vector<std::vector<std::string>> get_all_cmds_list();
    int get_command_by_name(const std::string& cmd);
    bool run_command_by_id(const std::string& cmd, const std::vector<std::string>& args);
}
