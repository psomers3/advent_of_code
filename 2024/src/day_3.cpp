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
    std::regex number_extractor("mul\\(([0-9]+),([0-9]+)\\)");
    std::regex op_extractor("(do\\(\\))|(don't\\(\\))");
    std::smatch match;
    int total_part_1 = 0;
    int total_part_2 = 0;
    int curr_pos = 0;
    std::string content_copy = file_contents;
    std::vector<std::tuple<int, std::string>> op_ranges = {{0, "do()"}};

    while (std::regex_search(content_copy, match, op_extractor))
    {
        curr_pos += match.position() + match.str().length();
        op_ranges.push_back({curr_pos, match.str()});
        content_copy = match.suffix();
    }
    op_ranges.push_back({file_contents.size(), "do()"});
    curr_pos = 0;
    content_copy = file_contents;
    while (std::regex_search(content_copy, match, number_extractor))
    {
        curr_pos += match.position() + match.str().length();
        for (int i = 0; i < op_ranges.size() - 1; i++)
        {
            if (curr_pos >= std::get<0>(op_ranges.at(i)) && curr_pos < std::get<0>(op_ranges.at(i + 1)) && "do()" == std::get<1>(op_ranges.at(i)))
            {
                total_part_2 += std::stoi(match[1]) * std::stoi(match[2]);
                break;
            }
        }
        total_part_1 += std::stoi(match[1]) * std::stoi(match[2]);
        content_copy = match.suffix();
    }
    std::cout << "total part 1: " << total_part_1 << std::endl;
    std::cout << "total part 2: " << total_part_2 << std::endl;
}