#include <iostream>
#include <fstream>
#include <tuple>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "file_loading.h"
#include <regex>
#include <set>
#include <algorithm>
#include <ranges>
#include <cmath>
#include <iterator>

int main(int argc, char* argv[])
{
    int total = 0;
    std::set<int> winner_numbers = {};
    std::set<int> drawn_numbers = {};
    std::vector<std::string> lines = {};
    std::unordered_map<int, int> card_counts = {};
    std::string line;
    std::ifstream infile(get_file(argc, argv));
    std::smatch sym_match;
    std::regex num_expr("([0-9]+ )");
    
    int i = 0;
    while (std::getline(infile, line))
    {   
        lines.push_back(line+" ");
        card_counts[i] = 1;
        i++;
    }
    i = 0;
    for (auto& line : lines){
        winner_numbers.clear();
        drawn_numbers.clear();
        auto matches = std::sregex_iterator(line.begin(), line.end(), num_expr);
        int bar_position = line.find_first_of('|');
        for (std::sregex_iterator match_ptr = matches; match_ptr != std::sregex_iterator(); ++match_ptr)
        {
            std::smatch match = *match_ptr;
            int match_number = std::stoi(match.str());
            int pos = match.position();
            if (pos < bar_position){
                winner_numbers.insert(match_number);
            } 
            else{
                drawn_numbers.insert(match_number);
            }
        }
        std::set<int> overlap = {};
        std::set_intersection(
            winner_numbers.begin(), 
            winner_numbers.end(), 
            drawn_numbers.begin(), 
            drawn_numbers.end(),
            std::inserter(overlap, overlap.end()));
        if (overlap.size() > 0){
            total += pow(2, overlap.size() - 1);
            for (int j = i+1; j < i + overlap.size()+1; j++){
                card_counts[j] += card_counts[i]; 
            }
        }
        i++;
    }
    int card_total = 0;
    for (int j = 0; j < card_counts.size(); j++){
        card_total += card_counts[j];
    }
    std::cout << "total: " << total << std::endl;
    std::cout << "num cards: " << card_total << std::endl;
    return 0;
}