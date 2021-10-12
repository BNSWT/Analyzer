/*
 * Created Date: 2021-10-12 21:04:13
 * Author: yuyangz
 */

#include <string>

using namespace std;

enum STATE{
    INIT, //Init state.
    OP, //The state when a certain word ends and the result is to be written into the vector.
    LETTER,
    NUM,
    RESERVED_WORD,
    IDENTIFIER,
    NUMBER,
    ASSIGN,
    CALCULATOR, 
    DELIMITER,
    DIVIDER,
    COMMENTOR
};


enum TYPE{
    RESERVED_WORD,
    IDENTIFIER,
    NUMBER,
    ASSIGN,
    CALCULATOR, 
    DELIMITER,
    DIVIDER,
    COMMENTOR
};

const string reservedWords[] = {
    "int",
    "void",
    "if",
    "else",
    "while",
    "return"
};

const string calculator[] = {
    "+",
    "-",
    "*",
    "/",
    "=",
    "==",
    ">",
    ">=",
    "<",
    "<=",
    "!="   
};

struct token{
    TYPE type;

};