#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

string letters = "abcdefghijklmnopqrstuvwxyz"; //if(letters.find_first_of(mychar) != string::npos) then it is a letter
string digits = "0123456789"; //if(digits.find_first_of(mychar) != string::npos) then it is a digit
string separators = "";
string operators = "";
unordered_set<string> keywords{"function", "integer", "boolean", "real", "if", "endif", "else",
                                "return", "print", "scan", "while", "endwhile", "true", "false"};

//hardcde id DFSM 2D array
int idDFSM[6][3] = {
    {2, 6, 6},
    {3, 4, 5},
    {3, 4, 5},
    {3, 4, 5},
    {3, 4, 5},
    {6, 6, 6}
};

//hardcode int DFSM 2D array

//hardcode real DFSM 2D array


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
int lexer(string filename, std::vector<pair<string, string>> &tokens) {
    ifstream codefile (filename);
    char mychar;

    while( codefile.is_open() ) {
        mychar = codefile.get();

        if(mychar == '['){
            //code to skip comments
            if(codefile.get() == '*'){  //if next char is '*', it is the start of a comment
                //find end of comment or end of file
                while(codefile.is_open()) {
                    //check if charcter starts end comment symbol
                    if(codefile.get() == '*'){
                        if(codefile.get() == ']'){          //check if complete end comment symbol
                            break;                  //if comment ended, break out of read loop
                        }
                    }
                }
            }
            else {              //else not start of comment, it's just illegal
                codefile.unget();       //go back to '['
                tokens.push_back(make_pair(string(1, mychar), "illegal"));
            }
        }
        else if(letters.find_first_of(tolower(mychar)) != string::npos){   //begin LA
            string token = ID_FSM(mychar, codefile);
            string lexeme = keywords.find(token) != keywords.end() ? "keyword" : "identifier";
            tokens.push_back(make_pair(token, lexeme));
        }
        else if(digits.find_first_of(mychar) != string::npos) {
            //call int/real FSM
        }
        else if(separators.find_first_of(mychar) != string::npos) {
            tokens.push_back(make_pair(string(1, mychar), "separator"));
        }
        else if(operators.find_first_of(mychar) != string::npos) {
            tokens.push_back(make_pair(string(1, mychar), "operator"));
        }
        else if(isspace(mychar)){
            //do nothing to skip
        }
        else {
            tokens.push_back(make_pair(string(1, mychar), "illegal"));
        }
    }
}

//implement FSM for the identifiers
string ID_FSM(char mychar, ifstream &codefile) {
    int state = 1;
    string token = string(1, mychar);

    while (true){
        if(letters.find_first_of(tolower(mychar)) != string::npos){ //if letter
            state = idDFSM[state - 1][0];
            token.push_back(mychar);
        }
        else if(digits.find_first_of(mychar) != string::npos){      //if digit
            state = idDFSM[state - 1][1];
            token.push_back(mychar);
        }
        else if(mychar == '-'){                                     //if -
            state = idDFSM[state - 1][2];
            token.push_back(mychar);
        }
        else {                                                      //else break bc invalid symbol
            break;
        }

        //add code to check if end of file
        mychar = codefile.get();
    }
    
    //if current character isn't whitespace, move file pointer back 1
    if(!isspace(mychar))
        codefile.unget();

    //check if accepting state
    if(state == 2 || state == 3 || state == 4 || state == 5)
        return token;   //should I change the FSM to return token string???
    else return "";
}

//implement FSM for the integers


//implement FSM for the real numbers

