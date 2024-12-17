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
    long total_part_1 = 0;
    long total_part_2 = 0;
    std::regex calibration_re("([0-9]+):");
    std::regex number_re(" ([0-9]+)");
    long calibration_val;
    long new_option;

    std::string line;
    std::smatch match;
    std::istringstream iss(file_contents);
    while (std::getline(iss, line))
    {
        if (!std::regex_search(line, match, calibration_re))
            continue;
        calibration_val = std::stol(match[1]);
        std::vector<int> values = {};
        std::vector<long> options = {};
        std::vector<long> options_tmp = {};
        auto matches = std::sregex_iterator(line.begin(), line.end(), number_re);
        for (std::sregex_iterator match_ptr = matches; match_ptr != std::sregex_iterator(); ++match_ptr)
        {
            std::smatch match = *match_ptr;
            std::string match_str = match.str();
            values.push_back(std::stoi(match_str));
        }
        options = {values.at(0)};
        for (int i = 1; i < values.size(); i++)
        {
            int val = values.at(i);
            options_tmp = {};
            for (auto opt : options)
            {
                new_option = val + opt;
                if (new_option <= calibration_val)
                    options_tmp.push_back(new_option);
                new_option = val * opt;
                if (new_option <= calibration_val)
                    options_tmp.push_back(new_option);
            }
            options = options_tmp;
        }
        if (std::find(options.begin(), options.end(), calibration_val) != options.end())
        {
            total_part_1 += calibration_val;
            continue;
        }

        options = {values.at(0)};
        for (int i = 1; i < values.size(); i++)
        {
            int val = values.at(i);
            options_tmp = {};
            for (auto opt : options)
            {
                new_option = val + opt;
                if (new_option <= calibration_val)
                    options_tmp.push_back(new_option);
                new_option = val * opt;
                if (new_option <= calibration_val)
                    options_tmp.push_back(new_option);
                new_option = std::stol(std::to_string(opt) + std::to_string(val));
                if (new_option <= calibration_val)
                    options_tmp.push_back(new_option);
            }
            options = options_tmp;
        }
        if (std::find(options.begin(), options.end(), calibration_val) != options.end())
        {
            total_part_2 += calibration_val;
        }
    }
    std::cout << "total part 1: " << total_part_1 << std::endl;
    std::cout << "total part 2: " << total_part_1 + total_part_2 << std::endl;
}