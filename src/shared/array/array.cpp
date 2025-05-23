#include "arcpch.h"
#include "array.h"

#include "strings/strings.h"

namespace array
{
    bool is_empty(const std::vector<std::string>& arr)
    {
        return arr.empty() || arr.size() == 0;
    }

    std::vector<std::string> reduce(const std::vector<std::string>& arr)
    {
        if (is_empty(arr))
            return {};

        std::vector<std::string> result;
        std::copy_if(arr.begin(), arr.end(), std::back_inserter(result), [](const std::string& str) { return !str.empty(); });

        return result;
    }

    // get only unique elements from the array
    std::vector<std::string> unique(const std::vector<std::string>& arr)
    {
        if (is_empty(arr))
            return {};

        std::vector<std::string> result = arr;

        // sort the array to group duplicates together
        std::sort(result.begin(), result.end());
        // move unique elements to the front
        std::vector<std::string>::iterator last = std::unique(result.begin(), result.end());
        // remove the remaining elements (duplicates)
        result.erase(last, result.end());

        return result;
    }

    // https://stackoverflow.com/a/70991343/18121288
    // https://github.com/SrijanSriv211/AOs/blob/AOs-2.7/src/shared/Utils/Array.cs#L52
    std::vector<std::string> trim(const std::vector<std::string>& arr)
    {
        if (is_empty(arr))
            return {};

        auto is_not_empty = [](const std::string& str) { return !strings::is_empty(str); };

        // find the index of the first non-empty string
        auto first_index = std::find_if(arr.begin(), arr.end(), is_not_empty);

        // nothing to return if it's all whitespace anyway
        if (first_index == arr.end())
            return {};

        // find the index of the last non-empty string
        auto last_index = std::find_if(arr.rbegin(), arr.rend(), is_not_empty).base();

        // create new vector with the trimmed elements
        return std::vector<std::string>(first_index, last_index);
    }

    std::vector<std::string> flat(const std::vector<std::vector<std::string>>& arr)
    {
        std::vector<std::string> flattened_array = {};

        for (size_t i = 0; i < arr.size(); i++)
            flattened_array.insert(flattened_array.end(), arr[i].begin(), arr[i].end());
        return flattened_array;
    }
}
