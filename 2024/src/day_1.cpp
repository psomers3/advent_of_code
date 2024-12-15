#include "file_loading.h"
#include <string>
#include <vector>
#include <regex>
#include <map>

int main(int argc, char* argv[]){
    std::string file_path = get_file(argc, argv);
    std::string file_contents = get_file_as_string(file_path);
    std::vector<int> list_1;
    std::vector<int> list_2;

    std::regex number_extractor("([0-9]+)[ ]+([0-9]+)");
    std::smatch match;
    int total = 0;
    std::string content_copy = file_contents;
    std::map<int, int> counts = {};
    int similarity = 0;

    while (std::regex_search(content_copy, match, number_extractor)){
        list_1.emplace_back(std::stoi(match[0]));
        list_2.emplace_back(std::stoi(match[2]));
        content_copy = match.suffix();
    }
    std::sort(list_1.begin(), list_1.end());
    std::sort(list_2.begin(), list_2.end());
    
    for (int i = 0; i < list_1.size(); i++){
        total += std::abs(list_2.at(i) - list_1.at(i));
    }
    std::cout << "diff total: " << total << std::endl;
    for (auto val2 : list_2){counts[val2]++;}
    for (auto val : list_1){similarity += counts[val] * val;}
    std::cout << "similarity total: " << similarity << std::endl;
}