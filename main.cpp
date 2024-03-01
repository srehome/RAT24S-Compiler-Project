#include <iostream>
#include <string>
#include <vector>
#include "LA.cpp"

int main () {
    //code to get file name string
    std::string filename;
    std::vector<std::pair<std::string, std::string>> tokenVector;

    std::cout << "Please enter a filename: ";
    std::cin >> filename;

    lexer(filename, tokenVector);

    return 0;
}