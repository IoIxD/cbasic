#include "compiler.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cctype>


std::string Compiler::escape_string(std::string msg) {
    std::stringstream result;
    int i =0;
    for(unsigned char ch : msg) {
        result << (int)ch;
        if(i < msg.length()-1) {
            result  << ", ";
        }
        i+=1;
    }
    return result.str();
}

std::string Compiler::translate_string(std::string msg) {
    std::stringstream result;

    for (unsigned char ch : msg) {
        if (ch == ' ') {
            // Replace spaces with underscores
            result << '_';
        }
        else if(ch <= 31) {
            result << "_"
                   << std::hex
                   << static_cast<int>(ch);
        }
        else if (isalpha(ch)) {
            // ASCII characters: convert to lowercase directly
            result << static_cast<char>(std::tolower(ch));
        }
        else {
            // Non-ASCII/Unicode bytes: format as \u00XX
            result << "_"
                   << std::hex
                   << static_cast<int>(ch);
        }
    }

    return result.str();
}
