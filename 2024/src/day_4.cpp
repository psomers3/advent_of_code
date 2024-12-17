#include "file_loading.h"
#include <string>
#include <vector>

int main(int argc, char *argv[])
{
    std::string file_path = get_file(argc, argv);
    std::string file_contents = get_file_as_string(file_path);
    int total_part_1 = 0;
    int total_part_2 = 0;
    std::vector<std::vector<char>> letters;

    std::string line;
    std::istringstream iss(file_contents);
    while (std::getline(iss, line))
    {
        std::string modded_line = "..." + line + "...";
        letters.push_back(std::vector<char>(modded_line.begin(), modded_line.end()));
    }
    int length = letters.at(0).size();
    for (int i = 0; i < 3; i++)
    {
        std::string padding = std::string(length, '.');
        letters.insert(letters.begin(), std::vector<char>(padding.begin(), padding.end()));
        letters.push_back(std::vector<char>(padding.begin(), padding.end()));
    }
    std::vector<std::vector<int>> positions_to_check({
        {0, -1, 0, -2, 0, -3},    // left
        {-1, -1, -2, -2, -3, -3}, // up left
        {-1, 0, -2, 0, -3, 0},    // up
        {-1, 1, -2, 2, -3, 3},    // up right
        {0, 1, 0, 2, 0, 3},       // right
        {1, 1, 2, 2, 3, 3},       // right down
        {1, 0, 2, 0, 3, 0},       // down
        {1, -1, 2, -2, 3, -3},    // left down
    });

    std::vector<std::vector<int>> mas_positions({
        {-1, -1, 1, 1},
        {-1, 1, 1, -1},
    });

    for (int i = 3; i < length - 3; i++)
    {
        for (int j = 3; j < letters.size() - 3; j++)
        {
            if (letters.at(i).at(j) == 'X')
            {
                for (auto positions : positions_to_check)
                {
                    if ("MAS" == std::string({
                                     letters.at(i + positions.at(0)).at(j + positions.at(1)),
                                     letters.at(i + positions.at(2)).at(j + positions.at(3)),
                                     letters.at(i + positions.at(4)).at(j + positions.at(5)),
                                 }))
                    {
                        total_part_1++;
                    }
                }
            }

            if (letters.at(i).at(j) != 'A')
                continue;
            std::string x1({letters.at(i - 1).at(j - 1), letters.at(i + 1).at(j + 1)});
            std::string x2({letters.at(i - 1).at(j + 1), letters.at(i + 1).at(j - 1)});
            if (x1 == "MS" || x1 == "SM")
            {
                if (x2 == "MS" || x2 == "SM")
                    total_part_2++;
            }
        }
    }

    std::cout << "total part 1: " << total_part_1 << std::endl;
    std::cout << "total part 2: " << total_part_2 << std::endl;
}