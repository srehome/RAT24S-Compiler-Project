//INCLUDES
#include <cstdio>
#include "LA.cpp"

//CONSTANTS
std::pair<string, string> LexemeTokenPair;
const bool PrintRules = true;
int line = 1;
int& lineNumber = line;

//DECLARATIONS???
void OptFunctionDefinitions(ifstream& codefile, FILE *outfile);

//RULE 1: <Rat24S> -> $ <Opt Function Definitions> $ <Opt Declaration List> $ <Statement List> $
void RAT24S(ifstream& codefile, FILE *outfile) {
    LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
    if(PrintRules) {
        fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        fprintf(outfile, "     <Rat24S> -> $ <Opt Function Definitions> $ <Opt Declaration List> $ <Statement List> $");
    }

    //check first $, call OptFunDef, else print error
    if(LexemeTokenPair.first == "$") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        OptFunctionDefinitions(codefile, outfile);
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: first '$' separator expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }

    //check second $, call OptDecList, else print error
    if(LexemeTokenPair.first == "$") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        OptDeclarationList(codefile, outfile);
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: second '$' separator expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }

    //check third $, call StateList, else print error
    if(LexemeTokenPair.first == "$") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        StatementList(codefile, outfile);
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: third '$' separator expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
    
    //check last $, else print error
    if(LexemeTokenPair.first == "$") {
        //check if eof (loop through spaces?), else error
        while(!codefile.eof()) {
            if(!isspace(codefile.get()) && !codefile.eof()) {
                if(PrintRules) fprintf(outfile, "Error Line Number %d: eof marker expected, received unexpected characters", lineNumber);
                exit(1);
            }
        }
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: last '$' separator expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 2: <Opt Function Definitions> -> <Function Definitions> | <Empty>
void OptFunctionDefinitions(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Opt Function Definitions> -> <Function Definitions> | <Empty>");
    if(LexemeTokenPair.first == "function") {
        FunctionDefinitions(codefile, outfile);
    }
    else {
        Empty(outfile);
    }
}

//RULE 3: <Function Definitions> -> <Function> <Function Definitions'>
void FunctionDefinitions(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Function Definitions> -> <Function> <Function Definitions'>");
    Function(codefile, outfile);
    FunctionDefinitionsPrime(codefile, outfile);
}

//RULE 3.5: <Function Definitions'> -> <Function Definitions> | <Empty>
void FunctionDefinitionsPrime(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Function Definitions'> -> <Function Definitions> | <Empty>");
    if(LexemeTokenPair.first == "function") {
        FunctionDefinitions(codefile, outfile);
    }
    else {
        Empty(outfile);
    }
}

//RULE 4: <Function> -> function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
void Function(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Function> -> function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>");
    //check for "function" keyword
    if(LexemeTokenPair.first == "function") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        //parse identifier
        if(LexemeTokenPair.second == "identifier"){
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);

            //check for "("
            if(LexemeTokenPair.first == "(") {
                LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
                //parse optional parameter list
                OptParameterList(codefile, outfile);
                //check for ")"
                if(LexemeTokenPair.first == ")") {
                    LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                    if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
                    //parse optional declaration list
                    OptDeclarationList(codefile, outfile);
                    //parse body
                    Body(codefile, outfile);
                }
                else {
                    if(PrintRules) fprintf(outfile, "Error Line Number %d: ')' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
                    exit(1);
                }
            }
            else {
                if(PrintRules) fprintf(outfile, "Error Line Number %d: '(' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
                exit(1);
            }
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d: identifier expexted; recived %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
            exit(1);
        }
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: 'function' keyword expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 5: <Opt Parameter List> -> <Parameter List> | <Empty>
void OptParameterList(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Opt Parameter List> -> <Parameter List> | <Empty>");
    if(LexemeTokenPair.second == "identifier") {
        ParameterList(codefile, outfile);
    }
    else {
        Empty(outfile);
    }
}

//RULE 6: <Parameter List> -> <Parameter> <Parameter List'>
void ParameterList(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Parameter List> -> <Parameter> <Parameter List'>");
    Parameter(codefile, outfile);
    ParameterList_(codefile, outfile);
}

//RULE 6.5: <Parameter List'> ->, <Parameter List> | <Empty>
void ParameterList_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Parameter List'> -> , <Parameter List> | <Empty>");
    if(LexemeTokenPair.first == ",") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        ParameterList(codefile, outfile);
    }
    else {
        Empty(outfile);
    }
}

//RULE 7: <Parameter> -> <IDs> <Qualifier>
void Parameter(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "<Parameter> -> <IDs> <Qualifier>");
    IDs(codefile, outfile);
    Qualifier(codefile, outfile);
}

//RULE 8: <Qualifier> -> integer | boolean | real
void Qualifier(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "<Qualifier> -> integer | boolean | real");
    if(LexemeTokenPair.first == "integer" || LexemeTokenPair.first == "boolean" || LexemeTokenPair.first == "real") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: 'integer', 'boolean', or 'real' keyword expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 9: <Body> ::= { < Statement List> }
void Body(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Body> -> { < Statement List> }");
    //check for "{"
    if(LexemeTokenPair.first == "{") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        //parse statement list
        StatementList(codefile, outfile);
        //check for "}"
        if(LexemeTokenPair.first == "}") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d: '}' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
            exit(1);
        }
    }
}

//RULE 10: <Opt Declaration List> ::= <Declaration List> | <Empty>
void OptDeclarationList(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Opt Declaration List> -> <Declaration List> | <Empty>");
    if(LexemeTokenPair.first == "integer" || LexemeTokenPair.first == "boolean" || LexemeTokenPair.first == "real") {
        DeclarationList(codefile, outfile);
    }
    else {
        Empty(outfile);
    }
}

//RULE 11: <Declaration List> -> <Declaration> ; <Declaration List'>
void DeclarationList(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Declaration List> -> <Declaration> ; <Declaration List'>");
    Declaration(codefile, outfile);
    //check for ";"
    if(LexemeTokenPair.first == ";") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        DeclarationList_(codefile, outfile);
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: ';' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 11.5: <Declaration List'> -> <Declaration List> | <Empty>
void DeclarationList_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Declaration List'> -> <Declaration List> | <Empty>");
    if(LexemeTokenPair.first == "integer" || LexemeTokenPair.first == "boolean" || LexemeTokenPair.first == "real") {
        DeclarationList(codefile, outfile);
    }
    else {
        Empty(outfile);
    }
}

//RULE 12: <Declaration> ::= <Qualifier > <IDs>
void Declaration(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Declaration> -> <Qualifier > <IDs>");
    //parse qualifier
    Qualifier(codefile, outfile);
    //parse IDs
    IDs(codefile, outfile);
}

//RULE 13: <IDs> -> <Identifier> <IDs'>
void IDs(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <IDs> -> <Identifier> <IDs'>");
    //parse identifier
    if(LexemeTokenPair.second == "identifier"){
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        IDs_(codefile, outfile);
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: identifier expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 13.5: <IDs'> -> , <IDs> | <Empty>
void IDs_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <IDs'> -> , <IDs> | <Empty>");
    if(LexemeTokenPair.first == ",") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        IDs(codefile, outfile);
    }
    else {
        Empty(outfile);
    }
}

//RULE 14: <Statement List> -> <Statement> <Statement List'>
void StatementList(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Statement List> -> <Statement> <Statement List'>");
    Statement(codefile, outfile);
    StatementList_(codefile, outfile);
}

//RULE 14.5: <Statement List'> -> <Statement List> | <Empty>
void StatementList_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Statement List'> -> <Statement List> | <Empty>");
    if(LexemeTokenPair.first == "{" ||
        LexemeTokenPair.second == "identifier" ||
        LexemeTokenPair.first == "if" ||
        LexemeTokenPair.first == "return" ||
        LexemeTokenPair.first == "print" ||
        LexemeTokenPair.first == "scan" ||
        LexemeTokenPair.first == "while") {
        StatementList(codefile, outfile);
    }
    else {
        Empty(outfile);
    }
}

//RULE 15: <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
void Statement(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");
    if(LexemeTokenPair.first == "if") {
        If(codefile, outfile);
    }
    else if(LexemeTokenPair.first == "while") {
        While(codefile, outfile);
    }
    else if(LexemeTokenPair.first == "print") {
        Print(codefile, outfile);
    }
    else if(LexemeTokenPair.first == "scan") {
        Scan(codefile, outfile);
    }
    else if(LexemeTokenPair.first == "return") {
        Return(codefile, outfile);
    }
    else if(LexemeTokenPair.second == "identifier") {
        Assign(codefile, outfile);
    }
    else if(LexemeTokenPair.first == "{") {
        Compound(codefile, outfile);
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: invalid statement; expected '{', identifier, 'if', 'return', 'print', 'scan', or, 'while'; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 16: <Compound> ::= { <Statement List> }
void Compound(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Compound> -> { <Statement List> }");
    //check for "{"
    if(LexemeTokenPair.first == "{") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        //parse statement list
        StatementList(codefile, outfile);
        //check for "}"
        if(LexemeTokenPair.first == "}") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d: '}' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
            exit(1);
        }
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: '{' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 17: <Assign> ::= <Identifier> = <Expression> ;
void Assign(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Assign> -> <Identifier> = <Expression> ;");
    //parse identifier
    if(LexemeTokenPair.second == "identifier") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        //check for "="
        if(LexemeTokenPair.first == "=") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
            //parse expression
            Expression(codefile, outfile);
            //check for ";"
            if(LexemeTokenPair.first == ";") {
                LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
            }
            else {
                if(PrintRules) fprintf(outfile, "Error Line Number %d: ';' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
                exit(1);
            }
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d: '=' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
            exit(1);
        }
    }
    
}

//RULE 18: <If> -> if ( <Condition> ) <Statement> <If'>
void If(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <If> -> if ( <Condition> ) <Statement> <If'>");
    if(LexemeTokenPair.first == "if") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        if(LexemeTokenPair.first == "(") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
            Condition(codefile, outfile);
            if(LexemeTokenPair.first == ")") {
                LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
                Statement(codefile, outfile);
                If_(codefile, outfile);
            }
            else {
                if(PrintRules) fprintf(outfile, "Error Line Number %d: ')' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
                exit(1);
            }
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d: '(' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
            exit(1);
        }
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: 'if' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 18.5: <If'> -> endif | else <Statement> endif
void If_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <If'> -> endif | else <Statement> endif");
    if(LexemeTokenPair.first == "endif") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
    }
    else if(LexemeTokenPair.first == "else") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        Statement(codefile, outfile);
        if(LexemeTokenPair.first == "endif") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d: 'endif' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
            exit(1);
        }
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: 'endif' or 'else' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 19: <Return> -> return <Return'>
void Return(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Return> -> return <Return'>");
    if(LexemeTokenPair.first == "return") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        Return_(codefile, outfile);
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: 'return' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 19.5: <Return'> -> ; | <Expression>;
void Return_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Return'> -> ; | <Expression>;");
    if(LexemeTokenPair.first == ";") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
    }
    else {
        Expression(codefile, outfile);
        if(LexemeTokenPair.first == ";") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d: ';' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
            exit(1);
        }
    }
}

//RULE 20: <Print> ::= print ( <Expression>);
void Print(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Print> -> print ( <Expression>);");
    if(LexemeTokenPair.first == "print") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        if(LexemeTokenPair.first == "(") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
            Expression(codefile, outfile);
            if(LexemeTokenPair.first == ")") {
                LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
                if(LexemeTokenPair.first == ";") {
                    LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                    if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
                }
                else {
                    if(PrintRules) fprintf(outfile, "Error Line Number %d: ';' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
                    exit(1);
                }
            }
            else {
                if(PrintRules) fprintf(outfile, "Error Line Number %d: ')' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
                exit(1);
            }
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d: '(' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
            exit(1);
        }
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: 'print' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 21: <Scan> ::= scan ( <IDs> );
void Scan(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Scan> -> scan ( <IDs> );");
    if(LexemeTokenPair.first == "scan") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        if(LexemeTokenPair.first == "(") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
            IDs(codefile, outfile);
            if(LexemeTokenPair.first == ")") {
                LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
                if(LexemeTokenPair.first == ";") {
                    LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                    if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
                }
                else {
                    if(PrintRules) fprintf(outfile, "Error Line Number %d: ';' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
                    exit(1);
                }
            }
            else {
                if(PrintRules) fprintf(outfile, "Error Line Number %d: ')' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
                exit(1);
            }
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d: '(' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
            exit(1);
        }
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: 'scan' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 22: <While> ::= while ( <Condition> ) <Statement> endwhile
void While(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <While> -> while ( <Condition> ) <Statement> endwhile");
    if(LexemeTokenPair.first == "while") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        if(LexemeTokenPair.first == "(") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
            Condition(codefile, outfile);
            if(LexemeTokenPair.first == ")") {
                LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
                Statement(codefile, outfile);
                if(LexemeTokenPair.first == "endwhile") {
                    LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                    if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
                }
                else {
                    if(PrintRules) fprintf(outfile, "Error Line Number %d: 'endwhile' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
                    exit(1);
                }
            }
            else {
                if(PrintRules) fprintf(outfile, "Error Line Number %d: ')' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
                exit(1);
            }
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d: '(' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
            exit(1);
        }
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: 'while' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 23: <Condition> ::= <Expression> <Relop> <Expression>
void Condition(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Condition> ::= <Expression> <Relop> <Expression>");
    //parse expression
    Expression(codefile, outfile);
    //parse Relop
    Relop(codefile, outfile);
    //parse expression
    Expression(codefile, outfile);
}

//RULE 24: <Relop> ::= == |!= | > | < | <= | =>
void Relop(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Relop> ::= == |!= | > | < | <= | =>");
    if(LexemeTokenPair.first == "==" ||
        LexemeTokenPair.first == "!=" ||
        LexemeTokenPair.first == ">" ||
        LexemeTokenPair.first == "<" ||
        LexemeTokenPair.first == "<=" ||
        LexemeTokenPair.first == ">=") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: ==,!=, >, <, <=, or => expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 25: <Expression> -> <Term> <Expression'>
void Expression(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Expression> ::= <Term> <Expression'>");
    //parse term
    Term(codefile, outfile);
    //parse Expression'
    Expression_(codefile, outfile);
}

//RULE 25.5: <Expression'> -> + <Term> <Expression'> | - <Term> <Expression'> | <Empty>
void Expression_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Expression'> ::= + <Term> <Expression'> | - <Term> <Expression'> | <Empty>");
    if(LexemeTokenPair.first == "+" || LexemeTokenPair.first == "-") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        //parse term
        Term(codefile, outfile);
        //parse Expression'
        Expression_(codefile, outfile);
    }
    else {
        Empty(outfile);
    }
}

//RULE 26: <Term> -> <Factor> <Term'>
void Term(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Term> ::= <Factor> <Term'>");
    //parse factor
    Factor(codefile, outfile);
    //parse Term'
    Term_(codefile, outfile);
}

//RULE 26.5: <Term'> -> * <Factor> <Term'> | / <Factor> <Term'> | <Empty>
void Term_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Term'> ::= * <Factor> <Term'> | / <Factor> <Term'> | <Empty>");
    if(LexemeTokenPair.first == "*" || LexemeTokenPair.first == "/") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        //parse factor
        Factor(codefile, outfile);
        //parse Term'
        Term_(codefile, outfile);
    }
    else {
        Empty(outfile);
    }
}

//RULE 27: <Factor> ::= - <Primary> | <Primary>
void Factor(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Factor> ::= - <Primary> | <Primary>");
    if(LexemeTokenPair.first == "-") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        //parse primary
        Primary(codefile, outfile);
    }
    else {
        //parse primary
        Primary(codefile, outfile);
    }
}

//RULE 28: <Primary> -> <Identifier><Primary'> | <Integer> | ( <Expression> ) | <Real> | true | false
void Primary(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Primary> -> <Identifier><Primary'> | <Integer> | ( <Expression> ) | <Real> | true | false");
    if(LexemeTokenPair.second == "identifier") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        //parse primary'
        Primary_(codefile, outfile);
    }
    else if(LexemeTokenPair.first == "(") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        //parse expression
        Expression(codefile, outfile);
        if(LexemeTokenPair.first == ")") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d: ')' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
            exit(1);
        }
    }
    else if(LexemeTokenPair.second == "integer" ||
            LexemeTokenPair.second == "real" ||
            LexemeTokenPair.first == "true" ||
            LexemeTokenPair.first == "false") {
                LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d: identifier, integer, real, 'true', 'false', or '(' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 28.5: <Primary'> -> ( <IDs> ) | <Empty>
void Primary_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Primary'> -> ( <IDs> ) | <Empty>");
    if(LexemeTokenPair.first == "(") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        //parse IDs
        IDs(codefile, outfile);
        if(LexemeTokenPair.first == ")") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d: ')' expected; received %s %s", lineNumber, LexemeTokenPair.first, LexemeTokenPair.second);
            exit(1);
        }
    }
    else {
        Empty(outfile);
    }
}

//RULE 29: <Empty> -> ε
void Empty(FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Empty> -> ε");
}

