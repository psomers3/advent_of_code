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


struct Range{
    long in;
    long out;
    long range;
};

long get_value_from_map(std::vector<Range> &range_list, long value);
std::vector<std::vector<long>> get_value_from_map(std::vector<Range> &range_list, std::vector<std::vector<long>> &in_ranges);

int main(int argc, char* argv[])
{
    std::vector<std::string> lines = {};
    std::smatch sym_match;
    std::regex seed_expr("(?:seeds: )([\\s\\d]+)");
    std::vector<std::regex> regexes = {
        std::regex("(?:seed-to-soil map:[\\S\\s])([\\s\\d]+)"),
        std::regex("(?:soil-to-fertilizer map:[\\S\\s])([\\s\\d]+)"),
        std::regex("(?:fertilizer-to-water map:[\\S\\s])([\\s\\d]+)"),
        std::regex("(?:water-to-light map:[\\S\\s])([\\s\\d]+)"),
        std::regex("(?:light-to-temperature map:[\\S\\s])([\\s\\d]+)"),
        std::regex("(?:temperature-to-humidity map:[\\S\\s])([\\s\\d]+)"),
        std::regex("(?:humidity-to-location map:[\\S\\s])([\\s\\d]+)"),
    };
    std::vector<std::vector<Range>*> maps;
    std::regex range_expr("([\\d ]+)");
    std::regex num_pair("([0-9]+ [0-9]+)");
    std::regex num_expr("([0-9]+)");
    std::string file_contents = get_file_as_string(get_file(argc, argv));

    // for each mapping section
    for (auto& map_regex : regexes){
        auto matches = std::sregex_iterator(file_contents.begin(), file_contents.end(), map_regex);
        std::vector<Range> *current_map = new std::vector<Range>();
        maps.push_back(current_map);
        std::string section_str = matches->str();
        auto range_matches = std::sregex_iterator(section_str.begin(), section_str.end(), range_expr);
        // for each map line
        for (std::sregex_iterator range_match_ptr = range_matches; range_match_ptr != std::sregex_iterator(); ++range_match_ptr){
            std::smatch line_match = *range_match_ptr;
            std::string line = line_match.str();
            if (line.length() == 0){continue;}
            auto number_matches = std::sregex_iterator(line.begin(), line.end(), num_expr);
            if (number_matches == std::sregex_iterator()){continue;}
            long end = std::stol(number_matches->str());
            number_matches++;
            long start = std::stol(number_matches->str());
            number_matches++;
            long count = std::stol(number_matches->str());
            current_map->emplace_back(Range{start, end, count});
        }
    }
    auto matches = std::sregex_iterator(file_contents.begin(), file_contents.end(), seed_expr);
    std::string seed_line = matches->str();
    auto seed_ptr = std::sregex_iterator(seed_line.begin(), seed_line.end(), num_expr);
    std::vector<long> locations = {};

    // part 1
    // ************************************************************************************************
    for (std::sregex_iterator seed_match = seed_ptr; seed_match != std::sregex_iterator(); ++seed_match){
        long seed = std::stol(seed_match->str());
        for (auto& map : maps ){
            seed = get_value_from_map(*map, seed);
        }
        locations.push_back(seed);
    }
    long minimum = *std::ranges::min_element(locations);
    std::cout << "min location: " << minimum << std::endl;

    // part 2
    // ************************************************************************************************
    seed_ptr = std::sregex_iterator(seed_line.begin(), seed_line.end(), num_pair);
    locations.clear();
    std::vector<std::vector<long>> seed_ranges = {};
    long start;
    long count;
    long end;
    for (std::sregex_iterator seed_pair = seed_ptr; seed_pair != std::sregex_iterator(); ++seed_pair){
        std::string seed_pair_str = seed_pair->str();
        auto seed_match = std::sregex_iterator(seed_pair_str.begin(), seed_pair_str.end(), num_expr);
        start = std::stol(seed_match->str());
        seed_match++;
        count = std::stol(seed_match->str());
        end = start + count;
        seed_ranges.push_back({start, end});
    }
    
    for (auto& map : maps ){
        seed_ranges = get_value_from_map(*map, seed_ranges);
    }
    minimum = std::ranges::min_element(seed_ranges, [](auto a, auto b){return a.at(0) < b.at(0);})->at(0);
    std::cout << "min location: " << minimum << std::endl;
    return 0;
}


long get_value_from_map(std::vector<Range> &range_list, long value){
    for (Range range : range_list){
        if (value >= range.in && value < range.in + range.range){
            return range.out + (value - range.in);
        }
    }
    return value;
}

/*
    Here we take advantage of the fact that all mappings are increasing and continuous for the mapped area.
    So each range of entries is parsed as chunks by looking over the overlapping ranges to each map range.
*/
std::vector<std::vector<long>> get_value_from_map(std::vector<Range> &range_list, std::vector<std::vector<long>> &in_ranges){
    std::vector<std::vector<long>> return_ranges = {};

    while (in_ranges.size() > 0){
        auto range_to_consume = in_ranges.back();
        in_ranges.pop_back();
        for (auto& map_range : range_list){
            if (range_to_consume.size() != 2){continue;}
            long map_end = map_range.in + map_range.range-1;
            // overlaps map range to the left
            if (range_to_consume.at(0) <= map_range.in && range_to_consume.at(1) < map_end && range_to_consume.at(1) > map_range.in){
                return_ranges.emplace_back(
                    std::vector<long>({get_value_from_map(range_list, map_range.in), get_value_from_map(range_list, range_to_consume.at(1))})
                );
                range_to_consume.at(1) = map_range.in;
            }
            // overlaps map range to the right
            else if(range_to_consume.at(0) > map_range.in && range_to_consume.at(0) < map_end && range_to_consume.at(1) > map_end){
                return_ranges.emplace_back(
                    std::vector<long>({get_value_from_map(range_list, range_to_consume.at(0)), get_value_from_map(range_list, map_end)})
                );
                range_to_consume.at(0) = map_end;
            }
            // overlaps within map range
            else if(range_to_consume.at(0) > map_range.in && range_to_consume.at(1) < map_end){
                return_ranges.emplace_back(
                    std::vector<long>({get_value_from_map(range_list, range_to_consume.at(0)), get_value_from_map(range_list, range_to_consume.at(1))})
                );
                range_to_consume.clear();
            }
            // overlaps over map range
            else if(range_to_consume.at(0) < map_range.in && range_to_consume.at(1) > map_end){
                return_ranges.emplace_back(
                    std::vector<long>({get_value_from_map(range_list, map_range.in), get_value_from_map(range_list, map_end)})
                );
                in_ranges.emplace_back(
                    std::vector<long>({range_to_consume.at(0), map_range.in})
                );
                in_ranges.emplace_back(
                    std::vector<long>({map_end, range_to_consume.at(1)})
                );
            }
        }
        if (range_to_consume.size() == 2){
            if (range_to_consume.at(0) != range_to_consume.at(1))
                return_ranges.push_back(range_to_consume);
        }
    }
    return return_ranges;
}