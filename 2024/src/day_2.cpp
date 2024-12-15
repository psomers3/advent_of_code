#include "file_loading.h"
#include <string>
#include <vector>
#include <regex>
#include <map>
#include <set>

bool is_safe(std::vector<int> line){
    bool increasing = line.at(1) - line.at(0) >= 0;
    bool safe = true;
    int num_pos = 0;
    int num_neg = 0;
    for (int i = 1; i < line.size();i++){
        int diff = line.at(i) - line.at(i-1);
        diff >= 0 ? num_pos++: num_neg++;
        if (((diff >= 0) != increasing) || (!(1 <= std::abs(diff) && std::abs(diff) <= 3))) {
            safe=false;
        }
    }
    return safe;
}

int main(int argc, char* argv[]){
    std::string file_path = get_file(argc, argv);
    std::string file_contents = get_file_as_string(file_path);
    std::regex number_extractor("([0-9]+)+");
    int total_safe_part_1 = 0;
    int total_safe_part_2 = 0;

    std::string line;
    std::istringstream iss(file_contents);
    while (std::getline(iss, line)){
        std::vector<int> report = {};
        std::vector<int> diffs = {};
        auto matches = std::sregex_iterator(line.begin(), line.end(), number_extractor);
        for (std::sregex_iterator match_ptr = matches; match_ptr != std::sregex_iterator(); ++match_ptr){
            std::smatch match = *match_ptr;
            std::string match_str = match.str();
            report.push_back(std::stoi(match_str));
        }
        bool safe = is_safe(report);
        if (safe) {
            total_safe_part_1++;
            total_safe_part_2++;
            continue;
        }
        
        for (int i = 0; i < report.size(); i++){
            std::vector<int> new_report(report);
            new_report.erase(new_report.begin() + i);
            if (is_safe(new_report)) {
                total_safe_part_2++;
                break;
            }
        }
    }

    std::cout << "total safe part 1: " << total_safe_part_1 << std::endl;
    std::cout << "total safe part 2: " << total_safe_part_2 << std::endl;
}