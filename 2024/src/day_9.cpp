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
    long checksum_1 = 0;
    long checksum_2 = 0;
    int block_pos = 0;
    int right_file_num = file_contents.length() / 2;
    int right_file_pos = (right_file_num * 2) + 1;
    if (file_contents.length() % 2 == 0)
    {
        right_file_num--;
    }
    else
    {
        right_file_pos--;
    }
    int left_file_pos = 0;
    int left_file_num = 0;
    int chunks_to_move = file_contents[right_file_pos] - '0';
    int chunks_to_fill = 0;

    // part I
    while (left_file_pos < right_file_pos)
    {
        if (left_file_pos % 2 == 0)
        {
            for (int i = 0; i < file_contents[left_file_pos] - '0'; i++)
            {
                checksum_1 += left_file_num * block_pos;
                block_pos++;
            }
            left_file_pos++;
            chunks_to_fill = file_contents[left_file_pos] - '0';
            continue;
        }
        else if (chunks_to_fill == 0)
        {
            left_file_pos++;
            left_file_num++;
            continue;
        }

        if (chunks_to_move == 0)
        {
            right_file_pos -= 2;
            chunks_to_move = file_contents[right_file_pos] - '0';
            right_file_num--;
            continue;
        }

        checksum_1 += block_pos * right_file_num;
        block_pos++;
        chunks_to_fill--;
        chunks_to_move--;
    }
    for (int i = 0; i < chunks_to_move; i++)
    {
        checksum_1 += right_file_num * block_pos;
        block_pos++;
    }

    // part II
    block_pos = 0;
    long block_size = 0;
    right_file_num = file_contents.length() / 2;
    right_file_pos = (right_file_num * 2) + 1;
    if (file_contents.length() % 2 == 0)
    {
        right_file_num--;
    }
    else
    {
        right_file_pos--;
    }
    left_file_pos = 1;
    block_pos = file_contents[0] - '0';
    long total_block_size = block_pos;
    left_file_num = 0;
    std::vector<std::tuple<int, int>> open_spaces;
    while (left_file_pos < file_contents.length())
    {
        block_size = file_contents[left_file_pos] - '0';
        total_block_size += block_size;
        if (block_size != 0 && (left_file_pos % 2) != 0)
        {
            open_spaces.push_back({block_pos, block_size});
        }
        block_pos += block_size;
        left_file_pos++;
    }
    block_pos = total_block_size;
    while (right_file_pos >= 0)
    {
        block_size = file_contents[right_file_pos] - '0';
        block_pos -= block_size;
        if (right_file_pos % 2 == 1)
        {
            right_file_pos--;
            continue;
        }

        bool moved = false;
        for (int i = 0; i < open_spaces.size(); i++)
        {
            if (std::get<0>(open_spaces.at(i)) >= block_pos)
            {
                break;
            }
            if (std::get<1>(open_spaces.at(i)) >= block_size)
            {
                moved = true;
                for (int j = 0; j < block_size; j++)
                {
                    checksum_2 += (std::get<0>(open_spaces.at(i)) + j) * right_file_num;
                }
                int diff = std::get<1>(open_spaces.at(i)) - block_size;
                if (diff == 0)
                {
                    open_spaces.erase(open_spaces.begin() + i);
                    break;
                }
                open_spaces.at(i) = {std::get<0>(open_spaces.at(i)) + block_size, diff};
                break;
            }
        }
        if (!moved)
        {
            for (int j = 0; j < block_size; j++)
            {
                checksum_2 += (block_pos + j) * right_file_num;
            }
        }

        right_file_num--;
        right_file_pos--;
    }

    std::cout << "total part 1: " << checksum_1 << std::endl;
    std::cout << "total part 2: " << checksum_2 << std::endl;
}