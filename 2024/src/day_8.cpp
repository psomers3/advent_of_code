#include "file_loading.h"
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <set>


int main(int argc, char *argv[])
{
    std::string file_path = get_file(argc, argv);
    std::string file_contents = get_file_as_string(file_path);
    long total_part_1 = 0;
    long total_part_2 = 0;
    std::regex antenna_re("([0-9a-zA-Z])");
    std::map<char, std::set<std::tuple<int, int>>> antennas;
    std::string line;
    std::smatch match;
    std::istringstream iss(file_contents);
    int i = 0;
    int j = 0;
    int max_j, max_i;
    while (std::getline(iss, line)){
        j = 0;
        max_j = line.length();
        while (std::regex_search(line, match, antenna_re)){
            antennas[match.str()[0]].insert({i, j+match.position()});
            line = match.suffix();
            j += match.position() + 1;
        }
        i++;
    }
    max_i = i;
    int delta_i, delta_j, new_i, new_j;
    std::set<std::tuple<int,int>> antinodes;
    std::set<std::tuple<int,int>> antinodes_part2;
    for (auto keyval: antennas){
        auto positions = keyval.second;
        for (auto pos: positions){
            for (auto p: positions){
                if (pos == p) continue;
                delta_i = std::get<0>(p) - std::get<0>(pos);
                delta_j = std::get<1>(p) - std::get<1>(pos);
                new_i = std::get<0>(p) + delta_i;
                new_j = std::get<1>(p) + delta_j;
                if (new_i < 0 || new_i >= max_i) continue;
                if (new_j < 0 || new_j >= max_j) continue;
                antinodes.insert({new_i,new_j});
            }
            for (auto p: positions){
                if (pos == p) continue;
                delta_i = std::get<0>(p) - std::get<0>(pos);
                delta_j = std::get<1>(p) - std::get<1>(pos);
                new_i = std::get<0>(pos) + delta_i;
                new_j = std::get<1>(pos) + delta_j;
                while (!((new_i < 0 || new_i >= max_i) || (new_j < 0 || new_j >= max_j))){
                    antinodes_part2.insert({new_i,new_j});
                    new_i = new_i + delta_i;
                    new_j = new_j + delta_j;
                }

            }
        }

    }
    std::cout << "total part 1: " << antinodes.size() << std::endl;
    std::cout << "total part 2: " << antinodes_part2.size() << std::endl;
}