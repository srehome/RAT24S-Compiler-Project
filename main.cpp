#include <iostream>
#include <iomanip>
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

    for(std::pair<std::string, std::string> token : tokenVector){
        std::cout << std::left << setw(15) << token.second << token.first << std::endl;
    }

    return 0;
}
/*  Helpful Code  */
/*
    isspace(mychar)   --  checks if character is whitespace (space, tab, newline, etc)
    tolower(mychar)   --  converts char to lower
    string(1, mychar) --  converts char to string (for adding to vector)
    Vector:
    Add elements to the vector
        myVector.push_back(make_pair(1, 3.14));
        myVector.push_back(make_pair(2, 2.718));
    Access and manipulate elements
        First element of the first pair: myVector[0].first
        Second element of the first pair: myVector[0].second
        First element of the second pair: myVector[1].first
        Second element of the second pair: myVector[1].second 
    Loop through vector
        for (const auto& pair : myVector) {
            cout << pair.first << ": " << pair.second << endl;
        }
*/