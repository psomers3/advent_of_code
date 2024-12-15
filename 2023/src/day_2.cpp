#include <iostream>
#include <fstream>
#include <tuple>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "file_loading.h"
#include <regex>


typedef std::map<std::string, int> game_result;

game_result ground_truth = {{"blue", 14}, {"green", 13}, {"red", 12}};

int main(int argc, char* argv[])
{
    int space_index;
    int total = 0;
    int power_total = 0;
    int game_power = 1;
    bool valid = false;
    std::vector<game_result> games = {};
    game_result results;
    std::string line;
    std::ifstream infile(get_file(argc, argv));
    std::smatch match;
    std::regex str_expr("([0-9]+) ([a-zA-Z]+)");
    
    while (std::getline(infile, line))
    {   
        game_result new_result = {};
        auto matches = std::sregex_iterator(line.begin(), line.end(), str_expr);
        for (std::sregex_iterator match_ptr = matches; match_ptr != std::sregex_iterator(); ++match_ptr)
        {
            std::smatch match = *match_ptr;
            std::string match_str = match.str();
            space_index = match_str.find(" ");
            std::string color = match_str.substr(space_index+1, match_str.size()-space_index);
            new_result.emplace(color, 0);
            new_result[color] = std::max({new_result[color], std::stoi(match_str.substr(0, space_index))});
        }
        games.push_back(new_result);
    }

    for (int i = 0; i < games.size(); i++){
        game_power = 1;
        valid = true;
        for (const auto &[key, value] : games.at(i)){
            if (ground_truth[key] < value && valid){
                valid = false;
            }
            game_power *= value;
        }
        power_total += game_power;
        if (valid){
            total += i+1;
        }
    }
    std::cout << "total: " << total << std::endl;
    std::cout << "power total: " << power_total << std::endl;
    return 0;
}