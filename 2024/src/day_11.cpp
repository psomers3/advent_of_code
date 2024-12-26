#include "file_loading.h"
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <set>

long get_num_stones(std::string val, int multiplication_level, std::map<std::tuple<std::string, int>, long> &seen, int max_multiplication)
{
    std::tuple<std::string, int> cache_key = {val, max_multiplication - multiplication_level};
    if (seen.contains(cache_key))
        return seen[cache_key];
    long num_stones = 0;
    if (multiplication_level == max_multiplication)
    {
        return 1;
    }

    if (val == "0")
    {
        num_stones += get_num_stones("1", multiplication_level + 1, seen, max_multiplication);
    }

    else if (val.length() % 2 == 0)
    {
        std::string split_stone = "";
        split_stone = "";
        for (int j = 0; j < val.length() / 2; j++)
        {
            split_stone += val.at(j);
        }
        num_stones += get_num_stones(split_stone, multiplication_level + 1, seen, max_multiplication);
        split_stone = "";
        for (int j = val.length() / 2; j < val.length(); j++)
        {
            if (val.at(j) == '0' && split_stone == "")
                continue;
            split_stone += val.at(j);
        }
        if (split_stone == "")
            split_stone = "0";
        num_stones += get_num_stones(split_stone, multiplication_level + 1, seen, max_multiplication);
    }
    else
    {
        num_stones += get_num_stones(std::to_string(std::stol(val) * 2024), multiplication_level + 1, seen, max_multiplication);
    }

    seen[cache_key] = num_stones;
    return num_stones;
}

int main(int argc, char *argv[])
{
    std::string file_path = get_file(argc, argv);
    std::string file_contents = get_file_as_string(file_path);
    std::vector<std::string> current_line;
    std::string line;
    long total_part_1 = 0;
    long total_part_2 = 0;
    std::istringstream iss(file_contents);
    std::map<std::tuple<std::string, int>, long> stones_to_return;
    std::getline(iss, line);
    std::regex number_re("[0-9]+");

    auto matches = std::sregex_iterator(line.begin(), line.end(), number_re);
    for (std::sregex_iterator match_ptr = matches; match_ptr != std::sregex_iterator(); ++match_ptr)
    {
        std::smatch match = *match_ptr;
        current_line.push_back(match.str());
    }
    std::string split_stone = "";
    for (auto stone : current_line)
    {
        total_part_1 += get_num_stones(stone, 0, stones_to_return, 25);
    }

    for (auto stone : current_line)
    {
        total_part_2 += get_num_stones(stone, 0, stones_to_return, 75);
    }
    std::cout << "total part 1: " << total_part_1 << std::endl;
    std::cout << "total part 2: " << total_part_2 << std::endl;
}