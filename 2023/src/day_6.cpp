#include <iostream>
#include <fstream>
#include <tuple>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include "file_loading.h"
#include <regex>
#include <algorithm>
#include <cmath>
#include <iterator>


struct Race{
    long time;
    long dist;
};
std::tuple<double, double> get_zeros(Race race);


int main(int argc, char* argv[])
{
    long total = 1;
    Race big_race;
    std::string big_time = "";
    std::string big_dist = "";
    std::vector<Race> races = {};
    std::string line;
    std::vector<std::string> lines;
    std::regex num_expr("([0-9]+)");
    std::ifstream infile(get_file(argc, argv));
    while (std::getline(infile, line)){lines.push_back(line + " ");}
    auto time_match = std::sregex_iterator(lines.at(0).begin(), lines.at(0).end(), num_expr);
    auto dist_match = std::sregex_iterator(lines.at(1).begin(), lines.at(1).end(), num_expr);
    while (time_match != std::sregex_iterator()){
        races.emplace_back(Race{std::stoi(time_match->str()), std::stoi(dist_match->str())});
        big_time += time_match->str();
        big_dist += dist_match->str();
        time_match++;
        dist_match++;
    }
    big_race = {std::stol(big_time), std::stol(big_dist)};
    std::cout << races.size() << " races" << std::endl;
    for (auto& race : races){
        auto zeros = get_zeros(race);
        if (fmod(std::get<1>(zeros), 1.0) == 0.0){
            std::get<1>(zeros) -= 1;
        }
        total *= floor(std::get<1>(zeros)) - floor(std::get<0>(zeros));
    }
    std::cout << "total: " << total << std::endl;

    auto zeros = get_zeros(big_race);
    if (fmod(std::get<1>(zeros), 1.0) == 0.0){
        std::get<1>(zeros) -= 1;
    }
    long big_total = floor(std::get<1>(zeros)) - floor(std::get<0>(zeros));
    std::cout << "big_total: " << big_total << std::endl;
    return 0;
}


std::tuple<double, double> get_zeros(Race race){
    double a = 1;
    double b = race.time;
    double c = race.dist;
    double val1 = abs((-b - sqrt(pow(b, 2) - 4*a*c))/(2*a));
    double val2 = abs((-b + sqrt(pow(b, 2) - 4*a*c))/(2*a));
    return std::tuple<double, double>(val1 < val2 ? val1 : val2, val1 < val2 ? val2 : val1);
}
// d = wait_time*(time - wait_time)
// wait_time^2 + wait_time*t - d = 0
// wait_time = (-b +- sqrt(b^2 - 4ac)) / 2*a