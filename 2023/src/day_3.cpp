#include <iostream>
#include <fstream>
#include <tuple>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "file_loading.h"
#include <regex>


struct NumberEntry
{
    std::string string;
    int row;
    int col;
};



int main(int argc, char* argv[])
{
    int total = 0;
    int gear_ratio_total = 0;
    std::vector<std::string> lines = {};
    std::vector<NumberEntry> potential_gears = {};
    std::string line;
    std::string substring;
    std::ifstream infile(get_file(argc, argv));
    std::smatch sym_match;
    std::regex num_expr("([0-9]+)");
    std::regex sym_expr("([^.[^0-9])");
    std::regex gear_ratio_expr("(\\*)");
    
    while (std::getline(infile, line))
    {   
        // pad edges with '.'
        line = "." + line + ".";
        if (lines.size() == 0){
            // pad first row with '.'
            lines.emplace_back(std::string(line.size(), '.'));
        }
        lines.push_back(line);
    }
    lines.emplace_back(std::string(lines.at(0).size(), '.'));

    for (int row=1; row < lines.size()-1; row++){
        line = lines.at(row);
        auto matches = std::sregex_iterator(line.begin(), line.end(), num_expr);

        // part one
        /*********************************************************************************************/

        for (std::sregex_iterator match_ptr = matches; match_ptr != std::sregex_iterator(); ++match_ptr)
        {
            std::smatch match = *match_ptr;
            std::string match_str = match.str();
            int pos = match.position();
            for (int check_row : {row-1, row, row+1}){
                substring = lines.at(check_row).substr(pos - 1, match_str.length()+2);
                if (std::regex_search(substring, sym_match, sym_expr)){
                    total += std::stoi(match_str);
                    break;
                }
            }
        }
        matches = std::sregex_iterator(line.begin(), line.end(), gear_ratio_expr);

        // part two
        /*********************************************************************************************/
        for (std::sregex_iterator match_ptr = matches; match_ptr != std::sregex_iterator(); ++match_ptr)
        {
            std::smatch match = *match_ptr;
            std::string match_str = match.str();
            potential_gears.clear();
            int pos = match.position();
            // check rows above and below
            for (int check_row : {row-1, row, row+1}){
                substring = lines.at(check_row).substr(pos - 1, match_str.length()+2);
                auto integer_matches = std::sregex_iterator(substring.begin(), substring.end(), num_expr);
                // find all groups of numbers (no breaks) and record their position in the global file lines
                for (std::sregex_iterator match_ptr2 = integer_matches; match_ptr2 != std::sregex_iterator(); ++match_ptr2){
                    std::smatch integer_match = *match_ptr2;
                    int global_pos = integer_match.position() + pos - 1;
                    NumberEntry new_entry = {"", check_row, global_pos};
                    char* letter = &lines.at(new_entry.row).at(new_entry.col);
                    // first find beginning of number
                    while (std::isdigit(*letter)){letter--;}
                    letter++;
                    // then read in number
                    while (std::isdigit(*letter)){
                        new_entry.string += *letter;
                        letter++;
                    }
                    potential_gears.push_back(new_entry);
                }
            }
            if (potential_gears.size() == 2){
                gear_ratio_total += std::stoi(potential_gears.at(0).string) * std::stoi(potential_gears.at(1).string);
            }
        }
    }
    std::cout << "total: " << total << std::endl;
    std::cout << "gear_total: " << gear_ratio_total << std::endl;
    return 0;
}