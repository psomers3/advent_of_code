#include <iostream>
#include <tuple>
#include <sstream>
#include <string>
#include "file_loading.h"
#include <vector>

const std::vector<std::tuple<std::string, char>> number_strings = {
    {"one", '1'},
    {"two", '2'},
    {"three", '3'},
    {"four", '4'},
    {"five", '5'},
    {"six", '6'},
    {"seven", '7'},
    {"eight", '8'},
    {"nine", '9'},
    {"zero", '0'}
};


int main(int argc, char* argv[])
{
    std::string line;
    char letter;
    std::string number1;
    uint16_t num1_pos;
    uint16_t num2_pos;
    int string_num1_pos;
    int string_num2_pos; 
    std::string number2;
    int total = 0;

    std::ifstream infile(get_file(argc, argv));
    while (std::getline(infile, line))
    {
        number1 = "";
        number2 = "";   
        for (int i = 0; i <= line.size(); i++){
            letter = line.at(i);
            if (std::isdigit(letter)){
                number1 = letter;
                num1_pos = i;
                break;
            }
        }
        for (int i = line.size() - 1; i >= -1; i--){
            letter = line.at(i);
            if (std::isdigit(letter)){
                number2 = letter;
                num2_pos = i;
                break;
            }
        }
        for (const auto& num_str : number_strings){
            string_num1_pos = line.find(std::get<0>(num_str));
            if (string_num1_pos != std::string::npos){
                if (string_num1_pos < num1_pos){
                    num1_pos = string_num1_pos;
                    number1 = std::get<1>(num_str);
                }
                while (string_num1_pos != std::string::npos){
                    string_num2_pos = string_num1_pos;
                    string_num1_pos = line.find(std::get<0>(num_str), string_num1_pos+1);
                }
                if (string_num2_pos > num2_pos){
                    num2_pos = string_num2_pos;
                    number2 = std::get<1>(num_str);
                }
            }
        }
        total += std::stoi(number1 + number2);
        std::cout << number1 + number2 << std::endl;
    }
    std::cout << "Your total is: " << total << std::endl;
    return 0;
}