#include "file_loading.h"
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <set>

std::tuple<int, int> get_area_and_perim(char c, int i, int j, std::vector<std::vector<char>> &garden, std::set<std::tuple<int, int>> &visited)
{
    if (visited.contains({i, j}))
    {
        return {0, 0};
    }
    if (i < 0 || j < 0 || i >= garden.size() || j >= garden.at(0).size())
    {
        return {0, 1};
    }
    if (garden.at(i).at(j) != c)
    {
        return {0, 1};
    }
    garden.at(i).at(j) = '.';
    visited.insert({i, j});
    int area = 0;
    int perim = 0;
    auto upper = get_area_and_perim(c, i - 1, j, garden, visited);
    auto lower = get_area_and_perim(c, i + 1, j, garden, visited);
    auto left = get_area_and_perim(c, i, j - 1, garden, visited);
    auto right = get_area_and_perim(c, i, j + 1, garden, visited);
    area += std::get<0>(upper) + std::get<0>(lower) + std::get<0>(right) + std::get<0>(left);
    perim += std::get<1>(upper) + std::get<1>(lower) + std::get<1>(right) + std::get<1>(left);
    return {area + 1, perim};
}

int main(int argc, char *argv[])
{
    std::string file_path = get_file(argc, argv);
    std::string file_contents = get_file_as_string(file_path);
    std::vector<std::vector<char>> garden;
    long total_part_1 = 0;
    long total_part_2 = 0;

    std::string line;
    std::istringstream iss(file_contents);
    std::vector<char> row;
    while (std::getline(iss, line))
    {
        garden.push_back(std::vector<char>(line.begin(), line.end()));
    }
    std::set<std::tuple<int, int>> visited;
    for (int i = 0; i < garden.size(); i++)
    {
        for (int j = 0; j < garden.at(0).size(); j++)
        {
            if (garden.at(i).at(j) != '.')
            {
                visited.clear();
                auto area_perim = get_area_and_perim(garden.at(i).at(j), i, j, garden, visited);
                total_part_1 += std::get<0>(area_perim) * std::get<1>(area_perim);
            }
        }
    }

    std::cout << "total part 1: " << total_part_1 << std::endl;
    std::cout << "total part 2: " << total_part_2 << std::endl;
}