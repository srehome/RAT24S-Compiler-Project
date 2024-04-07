#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>
#include <vector>
#include "SA.cpp"

int main () {
    std::string filename;

    //ask user to provide source code text file
    std::cout << "Please enter a source code text filename: ";
    std::cin >> filename;

    //Open the filestream
    ifstream codefile (filename);

    //if file didn't open, leave function
    if(!codefile.is_open())
        return -1;

    std::cout << "Please enter an output text filename: ";
    std::cin >> filename;

    FILE *outfile;
    outfile = fopen(filename.c_str(), "w");
    
    RAT24S(codefile, outfile);

    fclose(outfile); //close

    return 0;
}
