//INCLUDES
#include <cstdio>
#include "LA.cpp"

//CONSTANTS
std::pair<string, string> LexemeTokenPair;
const bool PrintRules = true;
int line = 1;
int& lineNumber = line;
int memory_address = 5000;
//vector<int> memory;           //store memory values; NOTE memory[MemoryLocation-5000]
//vector<int> stack;
string symbol_table[100][3];    //Identifier, MemoryLocation, Type
int instr_address = 0;
string instr_table[1000][3];    //Address, Operator (assembly instruction), Operand (register)

//DECLARATIONS
void RAT24S(ifstream& codefile, FILE *outfile);
void Qualifier(ifstream& codefile, FILE *outfile);
void Body(ifstream& codefile, FILE *outfile);
void OptDeclarationList(ifstream& codefile, FILE *outfile);
void DeclarationList(ifstream& codefile, FILE *outfile);
void DeclarationList_(ifstream& codefile, FILE *outfile);
void Declaration(ifstream& codefile, FILE *outfile);
void IDs(ifstream& codefile, FILE *outfile);
void IDs_(ifstream& codefile, FILE *outfile);
void StatementList(ifstream& codefile, FILE *outfile);
void StatementList_(ifstream& codefile, FILE *outfile);
void Statement(ifstream& codefile, FILE *outfile);
void Compound(ifstream& codefile, FILE *outfile);
void Assign(ifstream& codefile, FILE *outfile);
void If(ifstream& codefile, FILE *outfile);
void If_(ifstream& codefile, FILE *outfile);
void Return(ifstream& codefile, FILE *outfile);
void Return_(ifstream& codefile, FILE *outfile);
void Print(ifstream& codefile, FILE *outfile);
void Scan(ifstream& codefile, FILE *outfile);
void While(ifstream& codefile, FILE *outfile);
void Condition(ifstream& codefile, FILE *outfile);
void Relop(ifstream& codefile, FILE *outfile); //24
void Expression(ifstream& codefile, FILE *outfile); //25
void Expression_(ifstream& codefile, FILE *outfile); //25.5
void Term(ifstream& codefile, FILE *outfile); //26
void Term_(ifstream& codefile, FILE *outfile); //26.5
void Factor(ifstream& codefile, FILE *outfile); //27
void Primary(ifstream& codefile, FILE *outfile); //28
void Primary_(ifstream& codefile, FILE *outfile); //28.5
void Empty(FILE *outfile); //29

/*
WRITE THIS PSEUDOCODE IN CODE FOR INSTRUCTION TABLE!!!
Function generate_instruction(op, oprnd)
//instr_Address shows the current instruction Address that is global
{
Instr_table [instr_Address][1] = inst_Address;
Instr_table [instr_Address][2] = op;
Instr_table [instr_Address][3] = oprnd;
Instr_Address++;
};

WRITE THESE FUNCTIONS FOR SYMBOL TABLE!!!
You need to write a procedure that will check to see if a particular identifier is already in the
table, a procedure that will insert into the table and a procedure that will printout all identifiers in
the table. If an identifier is used without declaring it, then the parser should provide an error
message. Also, if an identifier is already in the table and wants to declare it for the second time,
then the parser should provide an error message. Also, you should check the type match.

*/


//RULE 1: <Rat24S> -> $ <Opt Function Definitions> $ <Opt Declaration List> $ <Statement List> $
void RAT24S(ifstream& codefile, FILE *outfile) {
    LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
    if(PrintRules) {
        fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        fprintf(outfile, "     <Rat24S> -> $ <Opt Function Definitions> $ <Opt Declaration List> $ <Statement List> $\n");
    }

    //check first $, call OptFunDef, else print error
    if(LexemeTokenPair.first == "$") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator before Optional Function Definitions\n   Expected: separator '$'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }

    //check second $, call OptDecList, else print error
    if(LexemeTokenPair.first == "$") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        OptDeclarationList(codefile, outfile);
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator between Optional Function Definitions and Optional Declaration List\n   Expected: separator '$'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }

    //check third $, call StateList, else print error
    if(LexemeTokenPair.first == "$") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        StatementList(codefile, outfile);
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator between Optional Declaration List and Statement List\n   Expected: separator '$'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }
    
    //check last $, else print error
    if(LexemeTokenPair.first == "$") {
        //check if eof (loop through spaces?), else error
        while(!codefile.eof()) {
            if(!isspace(codefile.get()) && !codefile.eof()) {
                if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Expected: eof marker\n   Received: unexpected characters", lineNumber);
                exit(1);
            }
        }
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after Statement List\n   Expected: separator '$'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }
}

//RULE 8: <Qualifier> -> integer | boolean | real
void Qualifier(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Qualifier> -> integer | boolean\n");
    if(LexemeTokenPair.first == "integer" || LexemeTokenPair.first == "boolean") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing keyword for qualifier\n   Expected: keyword 'integer' or 'boolean'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }
}

//RULE 9: <Body> -> { < Statement List> }
void Body(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Body> -> { < Statement List> }\n");
    //check for "{"
    if(LexemeTokenPair.first == "{") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        //parse statement list
        StatementList(codefile, outfile);
        //check for "}"
        if(LexemeTokenPair.first == "}") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after function body\n   Expected: separator '}'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            exit(1);
        }
    }
}

//RULE 10: <Opt Declaration List> -> <Declaration List> | <Empty>
void OptDeclarationList(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Opt Declaration List> -> <Declaration List> | <Empty>\n");
    if(LexemeTokenPair.first == "integer" || LexemeTokenPair.first == "boolean") {
        DeclarationList(codefile, outfile);
    }
    else {
        Empty(outfile);
    }
}

//RULE 11: <Declaration List> -> <Declaration> ; <Declaration List'>
void DeclarationList(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Declaration List> -> <Declaration> ; <Declaration List'>\n");
    Declaration(codefile, outfile);
    //check for ";"
    if(LexemeTokenPair.first == ";") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        DeclarationList_(codefile, outfile);
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after declaration\n   Expected: separator ';'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }
}

//RULE 11.5: <Declaration List'> -> <Declaration List> | <Empty>
void DeclarationList_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Declaration List'> -> <Declaration List> | <Empty>\n");
    if(LexemeTokenPair.first == "integer" || LexemeTokenPair.first == "boolean") {
        DeclarationList(codefile, outfile);
    }
    else {
        Empty(outfile);
    }
}

//RULE 12: <Declaration> -> <Qualifier > <IDs>
void Declaration(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Declaration> -> <Qualifier > <IDs>\n");
    //parse qualifier
    Qualifier(codefile, outfile);
    //parse IDs
    IDs(codefile, outfile);
}

//RULE 13: <IDs> -> <Identifier> <IDs'>
void IDs(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <IDs> -> <Identifier> <IDs'>\n");
    //parse identifier
    if(LexemeTokenPair.second == "identifier"){
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        IDs_(codefile, outfile);
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing identifier in IDs\n   Expected: identifier\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }
}

//RULE 13.5: <IDs'> -> , <IDs> | <Empty>
void IDs_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <IDs'> -> , <IDs> | <Empty>\n");
    if(LexemeTokenPair.first == ",") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        IDs(codefile, outfile);
    }
    else {
        Empty(outfile);
    }
}

//RULE 14: <Statement List> -> <Statement> <Statement List'>
void StatementList(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Statement List> -> <Statement> <Statement List'>\n");
    Statement(codefile, outfile);
    StatementList_(codefile, outfile);
}

//RULE 14.5: <Statement List'> -> <Statement List> | <Empty>
void StatementList_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Statement List'> -> <Statement List> | <Empty>\n");
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

//RULE 15: <Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>
void Statement(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>\n");
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
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Invalid token to begin statement\n   Expected: '{', identifier, 'if', 'return', 'print', 'scan', 'while'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }
}

//RULE 16: <Compound> -> { <Statement List> }
void Compound(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Compound> -> { <Statement List> }\n");
    //check for "{"
    if(LexemeTokenPair.first == "{") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        //parse statement list
        StatementList(codefile, outfile);
        //check for "}"
        if(LexemeTokenPair.first == "}") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after statement list in compound\n   Expected: separator '}'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            exit(1);
        }
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator before statement list in compound\n   Expected: separator '{'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }
}

//RULE 17: <Assign> -> <Identifier> = <Expression> ;
void Assign(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Assign> -> <Identifier> = <Expression> ;\n");
    //parse identifier
    if(LexemeTokenPair.second == "identifier") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        //check for "="
        if(LexemeTokenPair.first == "=") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            //parse expression
            Expression(codefile, outfile);
            //check for ";"
            if(LexemeTokenPair.first == ";") {
                LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            }
            else {
                if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after expression in assign\n   Expected: separator ';'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
                exit(1);
            }
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing operator after identifier in assign\n   Expected: operator '='\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            exit(1);
        }
    }
    
}

//RULE 18: <If> -> if ( <Condition> ) <Statement> <If'>
void If(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <If> -> if ( <Condition> ) <Statement> <If'>\n");
    if(LexemeTokenPair.first == "if") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        if(LexemeTokenPair.first == "(") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            Condition(codefile, outfile);
            if(LexemeTokenPair.first == ")") {
                LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
                Statement(codefile, outfile);
                If_(codefile, outfile);
            }
            else {
                if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after condition in if-statement\n   Expected: separator ')'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
                exit(1);
            }
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after if\n   Expected: separator '('\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            exit(1);
        }
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing keyword if\n   Expected: keyword 'if'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }
}

//RULE 18.5: <If'> -> endif | else <Statement> endif
void If_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <If'> -> endif | else <Statement> endif\n");
    if(LexemeTokenPair.first == "endif") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
    }
    else if(LexemeTokenPair.first == "else") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        Statement(codefile, outfile);
        if(LexemeTokenPair.first == "endif") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing keyword to end if-statement\n   Expected: keyword 'endif'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            exit(1);
        }
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing keyword to end or extend if-statement\n   Expected: keyword 'endif' or 'else'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }
}

//RULE 19: <Return> -> return <Return'>
void Return(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Return> -> return <Return'>\n");
    if(LexemeTokenPair.first == "return") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        Return_(codefile, outfile);
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing keyword for return-statement\n   Expected: keyword 'return'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }
}

//RULE 19.5: <Return'> -> ; | <Expression>;
void Return_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Return'> -> ; | <Expression>;\n");
    if(LexemeTokenPair.first == ";") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
    }
    else {
        Expression(codefile, outfile);
        if(LexemeTokenPair.first == ";") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after expression in return-statement\n   Expected: separator ';'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            exit(1);
        }
    }
}

//RULE 20: <Print> -> print ( <Expression>);
void Print(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Print> -> print ( <Expression> );\n");
    if(LexemeTokenPair.first == "print") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        if(LexemeTokenPair.first == "(") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            Expression(codefile, outfile);
            if(LexemeTokenPair.first == ")") {
                LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
                if(LexemeTokenPair.first == ";") {
                    LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                    if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
                }
                else {
                    if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after print-statement\n   Expected: separator ';'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
                    exit(1);
                }
            }
            else {
                if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after expression in print-statement\n   Expected: separator ')'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
                exit(1);
            }
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after print in print-statement\n   Expected: separator '('\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            exit(1);
        }
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing keyword for print-statement\n   Expected: keyword 'print'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }
}

//RULE 21: <Scan> -> scan ( <IDs> );
void Scan(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Scan> -> scan ( <IDs> );\n");
    if(LexemeTokenPair.first == "scan") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        if(LexemeTokenPair.first == "(") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            IDs(codefile, outfile);
            if(LexemeTokenPair.first == ")") {
                LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
                if(LexemeTokenPair.first == ";") {
                    LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                    if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
                }
                else {
                    if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after scan-statement\n   Expected: separator ';'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
                    exit(1);
                }
            }
            else {
                if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after IDs in scan-statement\n   Expected: separator ')'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
                exit(1);
            }
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after scan\n   Expected: separator '('\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            exit(1);
        }
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing keyword in scan-statement\n   Expected: keyword 'scan'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }
}

//RULE 22: <While> -> while ( <Condition> ) <Statement> endwhile
void While(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <While> -> while ( <Condition> ) <Statement> endwhile\n");
    if(LexemeTokenPair.first == "while") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        if(LexemeTokenPair.first == "(") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            Condition(codefile, outfile);
            if(LexemeTokenPair.first == ")") {
                LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
                Statement(codefile, outfile);
                if(LexemeTokenPair.first == "endwhile") {
                    LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                    if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
                }
                else {
                    if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing keyword to end while-statement\n   Expected: keyword 'endwhile'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
                    exit(1);
                }
            }
            else {
                if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after condition in while-statement\n   Expected: separator ')'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
                exit(1);
            }
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after while\n   Expected: separator '('\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            exit(1);
        }
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing keyword for while-statement\n   Expected: keyword 'while'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }
}

//RULE 23: <Condition> -> <Expression> <Relop> <Expression>
void Condition(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Condition> -> <Expression> <Relop> <Expression>\n");
    //parse expression
    Expression(codefile, outfile);
    //parse Relop
    Relop(codefile, outfile);
    //parse expression
    Expression(codefile, outfile);
}

//RULE 24: <Relop> -> == |!= | > | < | <= | =>
void Relop(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Relop> -> == |!= | > | < | <= | =>\n");
    if(LexemeTokenPair.first == "==" ||
        LexemeTokenPair.first == "!=" ||
        LexemeTokenPair.first == ">" ||
        LexemeTokenPair.first == "<" ||
        LexemeTokenPair.first == "<=" ||
        LexemeTokenPair.first == "=>") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Wrong token for relop\n   Expected: operator ==,!=, >, <, <=, or =>\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }
}

//RULE 25: <Expression> -> <Term> <Expression'>
void Expression(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Expression> -> <Term> <Expression'>\n");
    //parse term
    Term(codefile, outfile);
    //parse Expression'
    Expression_(codefile, outfile);
}

//RULE 25.5: <Expression'> -> + <Term> <Expression'> | - <Term> <Expression'> | <Empty>
void Expression_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Expression'> -> + <Term> <Expression'> | - <Term> <Expression'> | <Empty>\n");
    if(LexemeTokenPair.first == "+" || LexemeTokenPair.first == "-") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
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
    if(PrintRules) fprintf(outfile, "     <Term> -> <Factor> <Term'>\n");
    //parse factor
    Factor(codefile, outfile);
    //parse Term'
    Term_(codefile, outfile);
}

//RULE 26.5: <Term'> -> * <Factor> <Term'> | / <Factor> <Term'> | <Empty>
void Term_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Term'> -> * <Factor> <Term'> | / <Factor> <Term'> | <Empty>\n");
    if(LexemeTokenPair.first == "*" || LexemeTokenPair.first == "/") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        //parse factor
        Factor(codefile, outfile);
        //parse Term'
        Term_(codefile, outfile);
    }
    else {
        Empty(outfile);
    }
}

//RULE 27: <Factor> -> - <Primary> | <Primary>
void Factor(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Factor> -> - <Primary> | <Primary>\n");
    if(LexemeTokenPair.first == "-") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
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
    if(PrintRules) fprintf(outfile, "     <Primary> -> <Identifier><Primary'> | <Integer> | ( <Expression> ) | <Real> | true | false\n");
    if(LexemeTokenPair.second == "identifier") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        //parse primary'
        Primary_(codefile, outfile);
    }
    else if(LexemeTokenPair.first == "(") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        //parse expression
        Expression(codefile, outfile);
        if(LexemeTokenPair.first == ")") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after primary expression\n   Expected: separator ')'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            exit(1);
        }
    }
    else if(LexemeTokenPair.second == "integer" ||
            LexemeTokenPair.second == "real" ||
            LexemeTokenPair.first == "true" ||
            LexemeTokenPair.first == "false") {
                LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
                if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
    }
    else {
        if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Wrong token for primary\n   Expected: identifier, integer, real, 'true', 'false', or '('\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        exit(1);
    }
}

//RULE 28.5: <Primary'> -> ( <IDs> ) | <Empty>
void Primary_(ifstream& codefile, FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Primary'> -> ( <IDs> ) | <Empty>\n");
    if(LexemeTokenPair.first == "(") {
        LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
        if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        //parse IDs
        IDs(codefile, outfile);
        if(LexemeTokenPair.first == ")") {
            LexemeTokenPair = lexer(codefile.get(), codefile, lineNumber);
            if(PrintRules) fprintf(outfile, "Token: %s     Lexeme: %s\n", LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
        }
        else {
            if(PrintRules) fprintf(outfile, "Error Line Number %d:\n   Missing separator after IDs in primary\n   Expected: separator ')'\n   Received: %s %s\n", lineNumber, LexemeTokenPair.second.c_str(), LexemeTokenPair.first.c_str());
            exit(1);
        }
    }
    else {
        Empty(outfile);
    }
}

//RULE 29: <Empty> -> ε
void Empty(FILE *outfile) {
    if(PrintRules) fprintf(outfile, "     <Empty> -> ε\n");
}
