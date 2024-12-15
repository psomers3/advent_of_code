#include "file_loading.h"
#include <vector>
#include <stdexcept>

std::string get_file(int argc, char* argv[]){
    std::vector<std::string> args(argv + 1, argv + argc);
    if (args.size() == 0){
        std::cout << "need a file input :(\n";
        exit(0);
    }

    std::string input_file = args.at(0);
    std::cout <<"got file: "<< input_file << std::endl;
    return input_file;
}


std::string get_file_as_string(std::string filename){
    std::ifstream infile(filename);
    if (!infile.good()){
        std::cout << "file not found: " << filename << std::endl; 
        std::throw_with_nested(std::invalid_argument("file not found: " + filename ));
    }
    std::string str;
    infile.seekg(0, std::ios::end);   
    str.reserve(infile.tellg());
    infile.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(infile)),
                std::istreambuf_iterator<char>());
    return str;
}