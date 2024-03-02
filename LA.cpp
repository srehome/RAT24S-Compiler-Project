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
string ID_FSM(char, ifstream&);
pair<string, string> Int_Real_DFSM(char, ifstream&);
int lexer(string, vector<pair<string, string>>&);

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
    { 5, 4 }
};


//Lexical Analyzer function

int lexer(string filename, vector<pair<string, string>> &tokens) {
    //Open the file and create a variable to keep track of current character
    ifstream codefile (filename);
    char mychar;

    //while the file is not at the end, parse tokens, identify them, and add them to the vector
    while( codefile.is_open() && !codefile.eof() ) {
        //get next character in file
        mychar = codefile.get();

        if(mychar == '['){  //code to skip comments
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
            else {              //else not start of comment, it's just illegal
                codefile.unget();       //go back to '['
                tokens.push_back(make_pair(string(1, mychar), "illegal"));  //add '[' to list of token as illegal
            }
        }
        //check if character is in letters string
        else if(letters.find_first_of(tolower(mychar)) != string::npos){
            string token = ID_FSM(mychar, codefile);    //call id fsm to read in and return full token
            string lexeme = keywords.find(token) != keywords.end() ? "keyword" : "identifier";  //if found in keywords, lexeme set to keyword, else identifier
            tokens.push_back(make_pair(token, lexeme)); //add id/keyowrd to list of tokens
        }
        //check if character is in digits string
        else if(digits.find_first_of(mychar) != string::npos) {
            tokens.push_back(Int_Real_DFSM(mychar, codefile));  //add int/real to list of tokens
        }
        //check if character is in separators string
        else if(separators.find_first_of(mychar) != string::npos) {
            tokens.push_back(make_pair(string(1, mychar), "separator"));    //add separator to list of tokens
        }
        //check if character is in operators string
        else if(operators.find_first_of(mychar) != string::npos) {
            char tempOp = codefile.get(); //Scan next character & store into temporary value
            //check for == => != <= operators
            if (((mychar == '=') && (tempOp == '=' || tempOp == '>')) ||
                ((mychar == '!') && (tempOp == '=')) ||
                ((mychar == '<') && (tempOp == '='))) {
                    //add concatenated operator to list of tokens
                    tokens.push_back(make_pair(string(1, mychar) + string(1, tempOp), "operator"));
            }
            else { //add simple operator to list of tokens
                codefile.unget();
                tokens.push_back(make_pair(string(1, mychar), "operator"));
            }
        }
        //check if character is whitespace
        else if(isspace(mychar)){
            //do nothing to skip whitespace
        }
        //if character is not recognized above, add to list of tokens as an illegal
        else {
            tokens.push_back(make_pair(string(1, mychar), "illegal"));
        }
    }
}


//Implement the FSM for the identifiers

string ID_FSM(char mychar, ifstream &codefile) {
    int state = 1;
    string token = string(1, mychar);

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
    string token = string(1, mychar);

    while (true) {
    if (digits.find_first_of(mychar) != string::npos) {      //if character is in digits string
            state = idDFSM[state - 1][0];
            token.push_back(mychar);
        }
        else if (mychar == '.') {               //if character is '.'
            if (idDFSM[state - 1][1] == 5) {    //if new state would be 5, meaning we already have a '.'
                codefile.unget();               //unget second '.'
                break;                          //break because int/real token is done
            }

            state = idDFSM[state - 1][1];       //else change state & add '.' character
            token.push_back(mychar);
        }
        else {                                 //else break because invalid symbol
            break;
        }

        //add code to check if end of file
        if(!codefile.eof())
            mychar = codefile.get();
        else break;
    }

    //if current character isn't whitespace, move file pointer back 1
    if (!isspace(mychar))
        codefile.unget();

    //if state 3, meaning ends with '.', unget '.', & set state to int acceptance state
    if(state == 3){
        if (isspace(mychar))        //if pointing to whitespace, unget
            codefile.unget();
        codefile.unget();           //currently pointing to '.', unget
        token.pop_back();           //pop '.' off the end of the token string
        state = 2;                  //set state to accepting int
    }

    //check if accepting state for int or real
    if (state == 2)
        return make_pair(token, "integer");
    else if (state == 4)
        return make_pair(token, "real");
    else return make_pair(token, "not int/real");
}

