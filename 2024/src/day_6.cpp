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
    int total_part_1 = 0;
    int total_part_2 = 0;
    int num_turns = 0;
    std::vector<std::tuple<int,int>> directions({
        {-1, 0}, {0, 1}, {1, 0}, {0, -1}
    });
    std::vector<std::vector<char>> spaces;
    std::set<std::tuple<int,int>> visited_spaces = {};
    std::vector<std::tuple<int, int, int>> path;

    std::string line;
    std::istringstream iss(file_contents);
    int curr_pos_i;
    int curr_pos_j;
    int i = 1;  // 1 because will prepend padding
    while (std::getline(iss, line)){
        std::string modded_line = "e" + line + "e";
        spaces.push_back(std::vector<char>(modded_line.begin(), modded_line.end()));
        int j = modded_line.find('^');
        if (j != std::string::npos){
            curr_pos_i = i;
            curr_pos_j = j;
            visited_spaces.insert({i,j});
            path.push_back({curr_pos_i, curr_pos_j, num_turns});
        }
        i ++;
    }
    std::cout << "start " << curr_pos_i << "," << curr_pos_j << std::endl;
    int length = spaces.at(0).size();
    std::string padding = std::string(length, 'e');
    spaces.insert(spaces.begin(), std::vector<char>(padding.begin(), padding.end()));
    spaces.push_back(std::vector<char>(padding.begin(), padding.end()));

    std::tuple<int,int> curr_direction = directions.at(num_turns % 4);
    char next_space = spaces.at(curr_pos_i + std::get<0>(curr_direction)).at(curr_pos_j + std::get<1>(curr_direction));
    while ('e' != next_space){
        if (next_space == '#'){
            num_turns++;
            curr_direction = directions.at(num_turns % 4);
            next_space = spaces.at(curr_pos_i + std::get<0>(curr_direction)).at(curr_pos_j + std::get<1>(curr_direction));
            continue;
        }
        curr_pos_i += std::get<0>(curr_direction);
        curr_pos_j += std::get<1>(curr_direction);
        path.push_back({curr_pos_i, curr_pos_j, num_turns});
        visited_spaces.insert({curr_pos_i,curr_pos_j});
        next_space = spaces.at(curr_pos_i + std::get<0>(curr_direction)).at(curr_pos_j + std::get<1>(curr_direction));
    }


    std::set<std::tuple<int,int>> obstical_positions = {};
    // sadly just brute force putting the obstacle along the original path and resimulate
    for (auto pos : path){
        if (std::get<0>(pos) == std::get<0>(path.at(0)) && std::get<1>(pos) == std::get<1>(path.at(0))) continue;
        curr_pos_i = std::get<0>(path.at(0));
        curr_pos_j = std::get<1>(path.at(0));        
        spaces.at(std::get<0>(pos)).at(std::get<1>(pos)) = '#';
        curr_direction = {-1, 0};
        std::set<std::tuple<int,int,int, int>> visited_spaces_with_direction = {{curr_pos_i, curr_pos_j, std::get<0>(curr_direction), std::get<1>(curr_direction) }};
        next_space = spaces.at(curr_pos_i + std::get<0>(curr_direction)).at(curr_pos_j + std::get<1>(curr_direction));
        num_turns = 0;
        while ('e' != next_space){
            if (next_space == '#'){
                num_turns++;
                curr_direction = directions.at(num_turns % 4);
                next_space = spaces.at(curr_pos_i + std::get<0>(curr_direction)).at(curr_pos_j + std::get<1>(curr_direction));
                std::tuple<int,int,int,int> new_pos = {
                    curr_pos_i, curr_pos_j, std::get<0>(curr_direction), std::get<1>(curr_direction)
                };
                if (visited_spaces_with_direction.contains(new_pos)){
                    obstical_positions.insert({std::get<0>(pos), std::get<1>(pos)});
                    break;
                }

                visited_spaces_with_direction.insert(new_pos);
                continue;
            }
            curr_pos_i += std::get<0>(curr_direction);
            curr_pos_j += std::get<1>(curr_direction);

            std::tuple<int,int,int,int> new_pos = {
                curr_pos_i, curr_pos_j, std::get<0>(curr_direction), std::get<1>(curr_direction)
            };
            if (visited_spaces_with_direction.contains(new_pos)){
                obstical_positions.insert({std::get<0>(pos), std::get<1>(pos)});
                break;
            }

            visited_spaces_with_direction.insert(new_pos);
            next_space = spaces.at(curr_pos_i + std::get<0>(curr_direction)).at(curr_pos_j + std::get<1>(curr_direction));
        }
        spaces.at(std::get<0>(pos)).at(std::get<1>(pos)) = '.';
    }

    std::cout << "total part 1: " << visited_spaces.size() << std::endl;
    std::cout << "total part 2: " << obstical_positions.size() << std::endl;
}