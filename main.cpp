#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <string>
#include <vector>
#include "LA.cpp"

int main () {
    std::string filename;

    //tokenVector(lexeme, token);
    std::vector<std::pair<std::string, std::string>> tokenVector; 

    //ask user to provide source code text file
    std::cout << "Please enter a source code text filename: ";
    std::cin >> filename;

    //Open the filestream
    ifstream codefile (filename);

    //if file didn't open, leave function
    if(!codefile.is_open())
        return -1;

    //create a variable to keep track of current character
    char mychar = codefile.get();

    //while the file is not at the end, parse tokens, identify them, and add them to the vector
    while(!codefile.eof()) {
        //code to skip comments
        if(mychar == '['){  
            if(codefile.get() == '*'){  //if next char is '*', it is the start of a comment
                //find the end of the comment or the end of the file
                while(!codefile.eof()) {
                    //check if charcter starts end comment symbol
                    if(codefile.get() == '*'){
                        if(!codefile.eof()){            //if still not end of file
                            if(codefile.get() == ']'){  //check if complete end comment symbol
                                break;                  //if comment ended, break out of reading comment loop
                            }
                        }
                    }
                }
            }
            else {                      //else not start of comment, it's just illegal
                codefile.unget();       //go back to '['
                std::string token = "illegal";
                tokenVector.push_back(std::make_pair(std::string(1, mychar), token));  //add '[' to list of tokens as illegal
            }
        }
        //check if character is whitespace
        else if(isspace(mychar)){
            //do nothing to skip whitespace
        }
        //call lexer and add lexeme, token pair to vector
        else{
            tokenVector.push_back(lexer(mychar, codefile));
        }
        
        //get next character
        mychar = codefile.get();
    }

    //ask user to provide name of output text file
    std::cout << "Please enter an output text filename: ";
    std::cin >> filename;

    //open output text file to write stdout to it
    freopen(filename.c_str(), "w", stdout );

    std::cout << std::left << setw(15) << "Token" << "Lexeme" << std::endl;
    std::cout << "---------------------" << std::endl;

    //iterate through the vector and print contents to output file
    for(std::pair<std::string, std::string> token : tokenVector){
        std::cout << std::left << setw(15) << token.second << token.first << std::endl;
    }
    
    freopen("CON", "w", stdout);    //fclose (stdout);

    return 0;
}
