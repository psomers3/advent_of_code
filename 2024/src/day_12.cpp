#include "file_loading.h"
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <set>

typedef std::tuple<int, int> position;
typedef std::tuple<int, int, int> PerimeterPiece;

std::tuple<int, int> get_area_and_perim(
    char c,
    int i,
    int j,
    int side,
    std::vector<std::vector<char>> &garden,
    std::set<position> &visited,
    std::set<PerimeterPiece> &perimeter_pieces)
{
    if (visited.contains({i, j}))
    {
        return {0, 0};
    }
    if (i < 0 || j < 0 || i >= garden.size() || j >= garden.at(0).size() || garden.at(i).at(j) != c)
    {
        perimeter_pieces.insert({i, j, side});
        return {0, 1};
    }
    garden.at(i).at(j) = '.';
    visited.insert({i, j});
    int area = 0;
    int perim = 0;
    auto upper = get_area_and_perim(c, i - 1, j, 2, garden, visited, perimeter_pieces);
    auto lower = get_area_and_perim(c, i + 1, j, 0, garden, visited, perimeter_pieces);
    auto left = get_area_and_perim(c, i, j - 1, 3, garden, visited, perimeter_pieces);
    auto right = get_area_and_perim(c, i, j + 1, 1, garden, visited, perimeter_pieces);
    area += std::get<0>(upper) + std::get<0>(lower) + std::get<0>(right) + std::get<0>(left);
    perim += std::get<1>(upper) + std::get<1>(lower) + std::get<1>(right) + std::get<1>(left);
    return {area + 1, perim};
}

int number_of_sides(std::set<PerimeterPiece> perimeter_pieces)
{
    std::vector<std::vector<PerimeterPiece>> sides;
    std::vector<PerimeterPiece> side;
    int num_pieces;

    while (perimeter_pieces.size() > 0)
    {
        num_pieces = perimeter_pieces.size();
        side.clear();
        side.push_back(*perimeter_pieces.begin());
        perimeter_pieces.erase(perimeter_pieces.begin());
        if (std::get<2>(side.at(0)) == -1)
            continue;
        while (num_pieces != perimeter_pieces.size())
        {
            num_pieces = perimeter_pieces.size();
            for (auto existing_perim_piece : side)
            {
                for (auto p : perimeter_pieces)
                {
                    if (std::abs(std::get<0>(p) - std::get<0>(existing_perim_piece)) > 1 || std::abs(std::get<1>(p) - std::get<1>(existing_perim_piece)) > 1)
                        continue;
                    if (std::abs(std::get<0>(p) - std::get<0>(existing_perim_piece)) == 1 && std::get<1>(p) - std::get<1>(existing_perim_piece) != 0)
                        continue;
                    if (std::abs(std::get<0>(p) - std::get<0>(existing_perim_piece)) == 1 && std::get<1>(p) - std::get<1>(existing_perim_piece) != 0)
                        continue;
                    if (std::get<2>(existing_perim_piece) != std::get<2>(p))
                        continue;
                    if (std::abs(std::get<0>(p) - std::get<0>(existing_perim_piece)) == 1 &&
                        std::get<1>(p) != std::get<1>(existing_perim_piece))
                        continue;
                    if (std::abs(std::get<1>(p) - std::get<1>(existing_perim_piece)) == 1 &&
                        std::get<0>(p) != std::get<0>(existing_perim_piece))
                        continue;
                    side.push_back(p);
                }
            }
            for (auto p : side)
            {
                if (perimeter_pieces.find(p) != perimeter_pieces.end())
                    perimeter_pieces.erase(perimeter_pieces.find(p));
            }
        }
        sides.push_back(side);
    }

    return sides.size();
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
    std::set<PerimeterPiece> perimeter_pieces;
    for (int i = 0; i < garden.size(); i++)
    {
        for (int j = 0; j < garden.at(0).size(); j++)
        {
            if (garden.at(i).at(j) != '.')
            {
                visited.clear();
                perimeter_pieces.clear();
                auto area_perim = get_area_and_perim(garden.at(i).at(j), i, j, -1, garden, visited, perimeter_pieces);
                total_part_1 += std::get<0>(area_perim) * std::get<1>(area_perim);
                total_part_2 += std::get<0>(area_perim) * number_of_sides(perimeter_pieces);
            }
        }
    }

    std::cout << "total part 1: " << total_part_1 << std::endl;
    std::cout << "total part 2: " << total_part_2 << std::endl;
}