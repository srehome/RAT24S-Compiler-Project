//INCLUDES
#include "LA.cpp"

//CONSTANTS???
std::pair<string, string> LexemeTokenPair;
const bool PrintRules = true;

//DECLARATIONS???


//RULE 1: <Rat24S> ::= $ <Opt Function Definitions> $ <Opt Declaration List> $ <Statement List> $
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
        return;
    }

    //check second $, call OptDecList, else print error
    if(LexemeTokenPair.first == "$") {
        LexemeTokenPair = lexer(codefile.get(), codefile);
        printf("Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        OptDeclarationList(codefile);
    }
    else {
        if(PrintRules) printf("Error: second '$' separator expected; received %s %s", LexemeTokenPair.first, LexemeTokenPair.second);
        return;
    }

    //check third $, call StateList, else print error
    if(LexemeTokenPair.first == "$") {
        LexemeTokenPair = lexer(codefile.get(), codefile);
        printf("Token: %s     Lexeme: %s", LexemeTokenPair.second, LexemeTokenPair.first);
        StatementList(codefile);
    }
    else {
        if(PrintRules) printf("Error: third '$' separator expected; received %s %s", LexemeTokenPair.first, LexemeTokenPair.second);
        return;
    }
    
    //check last $, else print error
    if(LexemeTokenPair.first == "$") {
        //check if eof (loop through spaces?), else error
    }
    else {
        if(PrintRules) printf("Error: last '$' separator expected; received %s %s", LexemeTokenPair.first, LexemeTokenPair.second);
        return;
    }
}
//RULE 2: <Opt Function Definitions> ::= <Function Definitions> | <Empty>
void OptFunctionDefinitions(ifstream& codefile) {
    if(PrintRules) printf("     <Opt Function Definitions> ::= <Function Definitions> | <Empty>");
    FunctionDefinitions(codefile);
}

//RULE 3: <Function Definitions> -> <Function> <Function Definitions'>
void FunctionDefinitions(ifstream& codefile) {

}


//RULE 3.5: <Function Definitions'> -> <Function Definitions> | ε
void FunctionDefinitions_(ifstream& codefile) {

}

//RULE 4: <Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>
void Function(ifstream& codefile) {

}

//RULE 5: <Opt Parameter List> ::= <Parameter List> | <Empty>
void OptParameterList(ifstream& codefile) {

}

//RULE 6: <Parameter List> -> <Parameter> <Parameter List'>
void ParameterList(ifstream& codefile) {

}

//RULE 6.5: <Parameter List'> -> , <Parameter List> | ε
void ParameterList_(ifstream& codefile) {

}

//RULE 7: <Parameter> ::= <IDs > <Qualifier>
void Parameter(ifstream& codefile) {

}

//RULE 8: <Qualifier> ::= integer | boolean | real
void Qualifier(ifstream& codefile) {

}

//RULE 9: <Body> ::= { < Statement List> }
void Body(ifstream& codefile) {

}

//RULE 10: <Opt Declaration List> ::= <Declaration List> | <Empty>
void OptDeclarationList(ifstream& codefile) {

}

//RULE 11: <Declaration List> -> <Declaration> ; <Declaration List'>
void DeclarationList(ifstream& codefile) {

}

//RULE 11.5: <Declaration List'> -> <Declaration List> | ε
void DeclarationList_(ifstream& codefile) {

}

//RULE 12: <Declaration> ::= <Qualifier > <IDs>
void Declaration(ifstream& codefile) {

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

//RULE 15: <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
void Statement(ifstream& codefile) {

}

//RULE 16: <Compound> ::= { <Statement List> }
void Compound(ifstream& codefile) {

}

//RULE 17: <Assign> ::= <Identifier> = <Expression> ;
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

//RULE 20: <Print> ::= print ( <Expression>);
void Print(ifstream& codefile) {

}

//RULE 21: <Scan> ::= scan ( <IDs> );
void Scan(ifstream& codefile) {

}

//RULE 22: <While> ::= while ( <Condition> ) <Statement> endwhile
void While(ifstream& codefile) {

}

//RULE 23: <Condition> ::= <Expression> <Relop> <Expression>
void Condition(ifstream& codefile) {

}

//RULE 24: <Relop> ::= == | != | > | < | <= | =>
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

//RULE 27: <Factor> ::= - <Primary> | <Primary>
void Factor(ifstream& codefile) {

}

//RULE 28: <Primary> -> <Identifier><Primary'> | <Integer> | ( <Expression> ) | <Real> | true | false
void Primary(ifstream& codefile) {

}

//RULE 28.5: <Primary'> -> ( <IDs> ) | ε
void Primary_(ifstream& codefile) {

}

//RULE 29: <Empty> ::= ε
void Empty(ifstream& codefile) {

}

