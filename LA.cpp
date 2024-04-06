#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

//Lists of characters/words for lexer to compare current character/token with
string letters = "abcdefghijklmnopqrstuvwxyz";
string digits = "0123456789";
string separators = "$,;{}()";
string operators = "+-*/=<>!"; //Operator List: + - * / = == != <= => 
unordered_set<string> keywords{"function", "integer", "boolean", "real", "if", "endif", "else",
                                "return", "print", "scan", "while", "endwhile", "true", "false"};

//Function Declarations
string ID_FSM(char mychar, ifstream &codefile);
pair<string, string> Int_Real_DFSM(char mychar, ifstream &codefile);
pair<string, string> lexer(char mychar,  ifstream &codefile);

//hardcde id DFSM 2D array
int idDFSM[6][3] = {
    {2, 6, 6},
    {3, 4, 5},
    {3, 4, 5},
    {3, 4, 5},
    {3, 4, 5},
    {6, 6, 6}
};

//hardcode real DFSM 2D array
int realDFSM[5][2] = {
    { 2, 5 },
    { 2, 3 },
    { 4, 5 },
    { 4, 5 },
    { 5, 5 }
};


//Lexical Analyzer function

pair<string, string> lexer(char mychar, ifstream &codefile) {
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
                            } else codefile.unget();
                        }
                    }
                }
            }
            else {                      //else not start of comment, it's just illegal
                codefile.unget();       //go back to '['
                std::string token = "illegal";
                return std::make_pair(std::string(1, mychar), token);  //add '[' to list of tokens as illegal
            }
        }
        //check if character is whitespace
        else if(isspace(mychar)){
            //do nothing to skip whitespace
        }
        //identify and return lexeme, token pair
        else{
            //check if character is in letters string
            if(letters.find_first_of(tolower(mychar)) != string::npos){
                string lexeme = ID_FSM(mychar, codefile);    //call id fsm to read in and return full token
                string token = keywords.find(lexeme) != keywords.end() ? "keyword" : "identifier";  //if found in keywords, lexeme set to keyword, else identifier
                return make_pair(lexeme, token); //add id/keyowrd to list of tokens
            }
            //check if character is in digits string or is '.'
            else if(digits.find_first_of(mychar) != string::npos || mychar == '.') {
                return Int_Real_DFSM(mychar, codefile);  //add int/real to list of tokens
            }
            //check if character is in separators string
            else if(separators.find_first_of(mychar) != string::npos) {
                return make_pair(string(1, mychar), "separator");    //add separator to list of tokens
            }
            //check if character is in operators string
            else if(operators.find_first_of(mychar) != string::npos) {
                char tempOp = codefile.get(); //Scan next character & store into temporary value
                //check for == => != <= operators
                if (((mychar == '=') && (tempOp == '=' || tempOp == '>')) ||
                    ((mychar == '!') && (tempOp == '=')) ||
                    ((mychar == '<') && (tempOp == '='))) {
                        //add concatenated operator to list of tokens
                        return make_pair(string(1, mychar) + string(1, tempOp), "operator");
                }
                else { //add simple operator to list of tokens
                    codefile.unget();
                    return make_pair(string(1, mychar), "operator");
                }
            }
            //if character is not recognized above, add to list of tokens as an illegal
            else {
                return make_pair(string(1, mychar), "illegal");
            }
        }
        
        //get next character
        mychar = codefile.get();
    }
}


//Implement the FSM for the identifiers

string ID_FSM(char mychar, ifstream &codefile) {
    int state = 1;
    string token = "";

    while (true){
        if(letters.find_first_of(tolower(mychar)) != string::npos){ //if character in letters string
            state = idDFSM[state - 1][0];
            token.push_back(mychar);
        }
        else if(digits.find_first_of(mychar) != string::npos){      //if character in digits string
            state = idDFSM[state - 1][1];
            token.push_back(mychar);
        }
        else if(mychar == '_'){                                     //if character is '_'
            state = idDFSM[state - 1][2];
            token.push_back(mychar);
        }
        else {                                                      //else break because invalid symbol
            break;
        }

        //check if end of file
        if(!codefile.eof())
            mychar = codefile.get();
        else break;
    }
    
    //if current character isn't whitespace, move file pointer back 1
    if(!isspace(mychar))
        codefile.unget();

    //check if accepting state
    if(state == 2 || state == 3 || state == 4 || state == 5)
        return token;
    else return "";
}


//Implement combined FSM for integer/reals

pair<string, string> Int_Real_DFSM(char mychar, ifstream& codefile)
{
    int state = 1;
    string token = "";

    while (true) {
        if (digits.find_first_of(mychar) != string::npos) {      //if character is in digits string
            state = realDFSM[state - 1][0];
            token.push_back(mychar);
        }
        else if (mychar == '.') {               //if character is '.'
            state = realDFSM[state - 1][1];       //else change state & add '.' character
            token.push_back(mychar);
        }
        else if (!isspace(mychar) &&
                separators.find_first_of(mychar) == string::npos &&
                operators.find_first_of(mychar) == string::npos) {                  //else break because invalid symbol
            state = 5;
            token.push_back(mychar);
        }
        else break;

        //add code to check if end of file
        if(!codefile.eof())
            mychar = codefile.get();
        else break;
    }

    //if current character isn't whitespace, move file pointer back 1
    if (!isspace(mychar))
        codefile.unget();       //ungets illegal characters including second '.'


    //check if accepting state for int or real
    if (state == 2)
        return make_pair(token, "integer");
    else if (state == 4)
        return make_pair(token, "real");
    else
        return make_pair(token, "illegal");
}

