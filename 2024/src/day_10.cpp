#include "file_loading.h"
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <set>

int get_score(char prev_val, int i, int j, std::vector<std::vector<char>> &hiking_map, std::set<std::tuple<int, int>> &visited, int &distinct)
{
    if (i < 0 || j < 0 || i >= hiking_map.size() || j >= hiking_map.at(0).size())
        return 0;
    if (hiking_map.at(i).at(j) - prev_val != 1)
        return 0;
    if (hiking_map.at(i).at(j) == '9')
    {
        distinct += 1;
        if (visited.contains({i, j}))
        {
            return 0;
        }
        visited.insert({i, j});
        return 1;
    }
    char current_val = hiking_map.at(i).at(j);
    return get_score(current_val, i + 1, j, hiking_map, visited, distinct) + get_score(current_val, i - 1, j, hiking_map, visited, distinct) + get_score(current_val, i, j - 1, hiking_map, visited, distinct) + get_score(current_val, i, j + 1, hiking_map, visited, distinct);
}

int main(int argc, char *argv[])
{
    std::string file_path = get_file(argc, argv);
    std::string file_contents = get_file_as_string(file_path);
    std::vector<std::vector<char>> hiking_map;
    long total_part_1 = 0;
    long total_part_2 = 0;
    std::string line;
    std::istringstream iss(file_contents);
    while (std::getline(iss, line))
    {
        hiking_map.push_back(std::vector<char>(line.begin(), line.end()));
    }
    for (int i = 0; i < hiking_map.size(); i++)
    {
        for (int j = 0; j < hiking_map.at(0).size(); j++)
        {
            char current_val = hiking_map.at(i).at(j);
            if (current_val == '0')
            {
                std::set<std::tuple<int, int>> visited = {};
                int distinct = 0;
                total_part_1 += get_score(current_val, i + 1, j, hiking_map, visited, distinct) + get_score(current_val, i - 1, j, hiking_map, visited, distinct) + get_score(current_val, i, j - 1, hiking_map, visited, distinct) + get_score(current_val, i, j + 1, hiking_map, visited, distinct);
                total_part_2 += distinct;
            }
        }
    }

    std::cout << "total part 1: " << total_part_1 << std::endl;
    std::cout << "total part 2: " << total_part_2 << std::endl;
}