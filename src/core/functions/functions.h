#pragma once

namespace functions
{
	std::pair<std::function<void()>, bool> get_cmd_func(const std::string& cmd);
	std::pair<std::function<void(const std::vector<std::string>&)>, bool> get_cmd_args_func(const std::string& cmd);
	std::vector<std::string> get_all_cmds();
}
