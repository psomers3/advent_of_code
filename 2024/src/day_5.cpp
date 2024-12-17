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
    int total_part_1 = 0;
    int total_part_2 = 0;
    std::map<std::string, std::vector<std::string>> page_rules;
    std::regex page_ruling_re("([0-9]+)\\|([0-9]+)");
    std::regex number_re("[0-9]+");
    std::smatch match;
    std::string content_copy = file_contents;

    while (std::regex_search(content_copy, match, page_ruling_re))
    {
        page_rules[match[1].str()].push_back(match[2].str());
        content_copy = match.suffix();
    }

    std::string line;
    std::istringstream iss(file_contents);
    while (std::getline(iss, line))
    {
        if (line.find(',') == std::string::npos)
            continue;

        std::vector<std::string> pages = {};
        auto matches = std::sregex_iterator(line.begin(), line.end(), number_re);
        for (std::sregex_iterator match_ptr = matches; match_ptr != std::sregex_iterator(); ++match_ptr)
        {
            std::smatch match = *match_ptr;
            pages.push_back(match.str());
        }
        std::set<std::string> preceding_pages = {};
        bool is_valid = true;
        for (int i = 0; i < pages.size(); i++)
        {
            for (auto p : page_rules[pages.at(i)])
            {
                if (preceding_pages.contains(p))
                {
                    is_valid = false;
                    break;
                }
            }
            if (!is_valid)
                break;
            preceding_pages.insert(pages.at(i));
        }
        if (is_valid)
        {
            total_part_1 += std::stoi(pages.at(pages.size() / 2));
            continue;
        }

        while (!is_valid)
        {
            is_valid = true;
            preceding_pages = {};
            for (int i = 0; i < pages.size(); i++)
            {
                for (auto p : page_rules[pages.at(i)])
                {
                    if (preceding_pages.contains(p))
                    {
                        is_valid = false;
                        std::string curr_val = pages.at(i);
                        pages.at(i) = p;
                        for (int j = 0; j < i; j++)
                        {
                            if (pages.at(j) == p)
                            {
                                pages.at(j) = curr_val;
                                break;
                            }
                        }
                        break;
                    }
                }
                if (!is_valid)
                    break;
                preceding_pages.insert(pages.at(i));
            }
        }
        total_part_2 += std::stoi(pages.at(pages.size() / 2));
    }

    std::cout << "total part 1: " << total_part_1 << std::endl;
    std::cout << "total part 2: " << total_part_2 << std::endl;
}