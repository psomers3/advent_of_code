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
    std::vector<std::vector<char>> garden;
    long total_part_1 = 0;
    long total_part_2 = 0;
    std::string line;
    std::istringstream iss(file_contents);
    std::vector<char> row;
    while (std::getline(iss, line)){
        garden.push_back(std::vector<char>(line.begin(), line.end()));
    }

    std::cout << "total part 1: " << total_part_1 << std::endl;
    std::cout << "total part 2: " << total_part_2 << std::endl;
}