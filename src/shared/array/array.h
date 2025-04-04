#pragma once

namespace array
{
    bool is_empty(const std::vector<std::string>& arr);
    std::vector<std::string> reduce(const std::vector<std::string>& arr);
    std::vector<std::string> unique(const std::vector<std::string>& arr);
    std::vector<std::string> trim(const std::vector<std::string>& arr);
    std::vector<std::string> flat(const std::vector<std::vector<std::string>>& arr);
};
