//INCLUDES
#include "LA.cpp"

//CONSTANTS???
std::pair<string, string> LexemeTokenPair;
const bool PrintRules = true;

//DECLARATIONS???


//RULE 1: <Rat24S> -> $ <Opt Function Definitions> $ <Opt Declaration List> $ <Statement List> $
void RAT24S(ifstream& codefile) {
    LexemeTokenPair = lexer(codefile.get(), codefile);
    if(PrintRules) {
        printf("Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        printf("     <Rat24S> -> $ <Opt Function Definitions> $ <Opt Declaration List> $ <Statement List> $");
    }

    //check first $, call OptFunDef, else print error
    if(LexemeTokenPair.first == "$") {
        LexemeTokenPair = lexer(codefile.get(), codefile);
        if(PrintRules) printf("Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        OptFunctionDefinitions(codefile);
    }
    else {
        if(PrintRules) printf("Error: first '$' separator expected; received %s %s", LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }

    //check second $, call OptDecList, else print error
    if(LexemeTokenPair.first == "$") {
        LexemeTokenPair = lexer(codefile.get(), codefile);
        printf("Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        OptDeclarationList(codefile);
    }
    else {
        if(PrintRules) printf("Error: second '$' separator expected; received %s %s", LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }

    //check third $, call StateList, else print error
    if(LexemeTokenPair.first == "$") {
        LexemeTokenPair = lexer(codefile.get(), codefile);
        printf("Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        StatementList(codefile);
    }
    else {
        if(PrintRules) printf("Error: third '$' separator expected; received %s %s", LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
    
    //check last $, else print error
    if(LexemeTokenPair.first == "$") {
        //check if eof (loop through spaces?), else error
        while(!codefile.eof()) {
            if(!isspace(codefile.get()) && !codefile.eof()) {
                if(PrintRules) printf("Error: eof marker expected, received unexpected characters");
                exit(1);
            }
        }
    }
    else {
        if(PrintRules) printf("Error: last '$' separator expected; received %s %s", LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 2: <Opt Function Definitions> -> <Function Definitions> | <Empty>
void OptFunctionDefinitions(ifstream& codefile) {
    if(PrintRules) printf("     <Opt Function Definitions> -> <Function Definitions> | <Empty>");
    FunctionDefinitions(codefile);
}

//RULE 3: <Function Definitions> -> <Function> <Function Definitions'>
void FunctionDefinitions(ifstream& codefile) {
    if(PrintRules) printf("     <Function Definitions> -> <Function> <Function Definitions'>");
    Function(codefile);
    FunctionDefinitionsPrime(codefile);
}

//RULE 3.5: <Function Definitions'> -> <Function Definitions> | ε
void FunctionDefinitionsPrime(ifstream& codefile) {
    if(PrintRules) printf("     <Function Definitions'> -> <Function Definitions> | ε");
    if(LexemeTokenPair.first == "function") {
        FunctionDefinitions(codefile);
    }
    else {
        //epsilon
    }
}

//RULE 4: <Function> -> function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
void Function(ifstream& codefile) {
    if(PrintRules) printf("     <Function> -> function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>");
    //check for "function" keyword
    if(LexemeTokenPair.first == "function") {
        LexemeTokenPair = lexer(codefile.get(), codefile);
        if(PrintRules) printf("Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        //parse identifier
        if(LexemeTokenPair.second == "identifier"){
            LexemeTokenPair = lexer(codefile.get(), codefile);
            printf("Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);

            //check for "("
            if(LexemeTokenPair.first == "(") {
                LexemeTokenPair = lexer(codefile.get(), codefile);
                if(PrintRules) printf("Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
                //parse optional parameter list
                OptParameterList(codefile);
                //check for ")"
                if(LexemeTokenPair.first == ")") {
                    LexemeTokenPair = lexer(codefile.get(), codefile);
                    if(PrintRules) printf("Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
                    //parse optional declaration list
                    OptDeclarationList(codefile);
                    //parse body
                    Body(codefile);
                }
                else {
                    if(PrintRules) printf("Error: ')' expected; received %s %s", LexemeTokenPair.first, LexemeTokenPair.second);
                    exit(1);
                }
            }
            else {
                if(PrintRules) printf("Error: '(' expected; received %s %s", LexemeTokenPair.first, LexemeTokenPair.second);
                exit(1);
            }
        }
        else {
            if(PrintRules) printf("Error: identifier expexted; recived %s %s", LexemeTokenPair.first, LexemeTokenPair.second);
            exit(1);
        }
    }
    else {
        if(PrintRules) printf("Error: 'function' keyword expected; received %s %s", LexemeTokenPair.first, LexemeTokenPair.second);
        exit(1);
    }
}

//RULE 5: <Opt Parameter List> -> <Parameter List> | <Empty>
void OptParameterList(ifstream& codefile) {
    if(PrintRules) printf("     <Opt Parameter List> -> <Parameter List> | <Empty>");
    if(LexemeTokenPair.first == "integer" || LexemeTokenPair.first == "boolean" || LexemeTokenPair.first == "real") {
        ParameterList(codefile);
    }
    else {
        //epsilon
    }
}

//RULE 6: <Parameter List> -> <Parameter> <Parameter List'>
void ParameterList(ifstream& codefile) {
    if(PrintRules) printf("     <Parameter List> -> <Parameter> <Parameter List'>");
    Parameter(codefile);
    ParameterList_(codefile);
}

//RULE 6.5: <Parameter List'> ->, <Parameter List> | ε
void ParameterList_(ifstream& codefile) {
    if(PrintRules) printf("     <Parameter List'> -> , <Parameter List> | ε");
    if(LexemeTokenPair.first == ",") {
        LexemeTokenPair = lexer(codefile.get(), codefile);
        if(PrintRules) printf("Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        ParameterList(codefile);
    }
    else {
        //epsilon
    }
}

//RULE 7: <Parameter> -> <IDs> <Qualifier>
void Parameter(ifstream& codefile) {

}

//RULE 8: <Qualifier> -> integer | boolean | real
void Qualifier(ifstream& codefile) {

}

//RULE 9: <Body> ::= { < Statement List> }
void Body(ifstream& codefile) {
    if(PrintRules) printf("     <Body> ::= { < Statement List> }");
    //check for "{"
    LexemeTokenPair = lexer(codefile.get(), codefile);
    if(LexemeTokenPair.first == "{") {
        if(PrintRules) printf("Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        //parse statement list
        StatementList(codefile);
        //check for "}"
        LexemeTokenPair = lexer(codefile.get(), codefile);
        if(LexemeTokenPair.first == "}") {
            if(PrintRules) printf("Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        }
        else {
            if(PrintRules) printf("Error: '}' expected; received %s %s", LexemeTokenPair.first, LexemeTokenPair.second);
            return;
        }
    }


//RULE 10: <Opt Declaration List> ::= <Declaration List> | <Empty>
void OptDeclarationList(ifstream& codefile) {
    if(PrintRules) printf("     <Opt Declaration List> ::= <Declaration List> | <Empty>");
    LexemeTokenPair = lexer(codefile.get(), codefile);
    if(LexemeTokenPair.first == "integer" || LexemeTokenPair.first == "boolean" || LexemeTokenPair.first == "real") {
        DeclarationList(codefile, LexemeTokenPair);
    }
    else {
        //epsilon
    }
}

//RULE 11: <Declaration List> -> <Declaration> ; <Declaration List'>
void DeclarationList(ifstream& codefile) {
    if(PrintRules) printf("     <Declaration List> -> <Declaration> ; <Declaration List'>");
    Declaration(codefile, LexemeTokenPair);
    //check for ";"
    if(LexemeTokenPair.first == ";") {
        LexemeTokenPair = lexer(codefile.get(), codefile);
        if(PrintRules) printf("Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        DeclarationList_(codefile, LexemeTokenPair);
    }
    else {
        if(PrintRules) printf("Error: ';' expected; received %s %s", LexemeTokenPair.first, LexemeTokenPair.second);
        return;
    }
}

//RULE 11.5: <Declaration List'> -> <Declaration List> | ε
void DeclarationList_(ifstream& codefile) {
    if(PrintRules) printf("     <Declaration List'> -> <Declaration List> | ε");
    LexemeTokenPair = lexer(codefile.get(), codefile);
    if(LexemeTokenPair.first == "integer" || LexemeTokenPair.first == "boolean" || LexemeTokenPair.first == "real") {
        DeclarationList(codefile, LexemeTokenPair);
    }
    else {
        //epsilon
    }
}

//RULE 12: <Declaration> ::= <Qualifier > <IDs>
void Declaration(ifstream& codefile) {
    if(PrintRules) printf("     <Declaration> ::= <Qualifier > <IDs>");
    //parse qualifier
    Qualifier(codefile, LexemeTokenPair);
    //parse IDs
    IDs(codefile, LexemeTokenPair);
}

//RULE 13: <IDs> -> <Identifier> <IDs'>
void IDs(ifstream& codefile) {

}

//RULE 13.5: <IDs'> -> , <IDs> | ε
void IDs_(ifstream& codefile) {

}

//RULE 14: <Statement List> -> <Statement> <Statement List'>
void StatementList(ifstream& codefile) {

}

//RULE 14.5: <Statement List'> -> <Statement List> | ε
void StatementList_(ifstream& codefile) {

}

//RULE 15: <Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
void Statement(ifstream& codefile) {

}

//RULE 16: <Compound> -> { <Statement List> }
void Compound(ifstream& codefile) {

}

//RULE 17: <Assign> -> <Identifier> = <Expression> ;
void Assign(ifstream& codefile) {

}

//RULE 18: <If> -> if ( <Condition> ) <Statement> <If'>
void If(ifstream& codefile) {

}

//RULE 18.5: <If'> -> endif | else <Statement> endif
void If_(ifstream& codefile) {

}

//RULE 19: <Return> -> return <Return'>
void Return(ifstream& codefile) {

}

//RULE 19.5: <Return'> -> ; | <Expression>;
void Return_(ifstream& codefile) {

}

//RULE 20: <Print> -> print ( <Expression>);
void Print(ifstream& codefile) {

}

//RULE 21: <Scan> -> scan ( <IDs> );
void Scan(ifstream& codefile) {

}

//RULE 22: <While> -> while ( <Condition> ) <Statement> endwhile
void While(ifstream& codefile) {

}

//RULE 23: <Condition> -> <Expression> <Relop> <Expression>
void Condition(ifstream& codefile) {

}

//RULE 24: <Relop> -> == | != | > | < | <= | =>
void Relop(ifstream& codefile) {

}

//RULE 25: <Expression> -> <Term> <Expression'>
void Expression(ifstream& codefile) {

}

//RULE 25.5: <Expression'> -> + <Term> <Expression'> | - <Term> <Expression'> | ε
void Expression_(ifstream& codefile) {

}

//RULE 26: <Term> -> <Factor> <Term'>
void Term(ifstream& codefile) {

}

//RULE 26.5: <Term'> -> * <Factor> <Term'> | / <Factor> <Term'> | ε
void Term_(ifstream& codefile) {

}

//RULE 27: <Factor> -> - <Primary> | <Primary>
void Factor(ifstream& codefile) {

}

//RULE 28: <Primary> -> <Identifier><Primary'> | <Integer> | ( <Expression> ) | <Real> | true | false
void Primary(ifstream& codefile) {

}

//RULE 28.5: <Primary'> -> ( <IDs> ) | ε
void Primary_(ifstream& codefile) {

}

//RULE 29: <Empty> -> ε
void Empty(ifstream& codefile) {

}

