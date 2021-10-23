#pragma once
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
    SYMBOL
};


enum TYPE{
    RESERVED_WORD,
    IDENTIFIER,
    NUMBER,
    ASSIGN,
    CALCULATOR, 
    DELIMITER,
    DIVIDER
};

enum INPUT_TYPE{
    LETTER_INPUT,
    NUMBER_INPUT,
    SYMBOL_INPUT,
    SPACE_INPUT
};

enum SYMBOL_TYPE{
    MUST_SINGLE, 
    MAY_DOUBLE,
    NOT_INCLUDED
};

const string type[] = {
    "reserved_word",
    "identifier",
    "number",
    "assign",
    "calculator",
    "delimiter",
    "divider",
    ""
};

const string reservedWords[] = {
    "int",
    "main",
    "void",
    "if",
    "else",
    "while",
    "return",
    ""
};

const char mustSingleSymbol[] = {
    '+',
    '-',
    '*',
    '/',
    '{',
    '}',
    '(',
    ')',
    ',',
    ';',
    0
};

const char mayDoubleSymbol[] = {
    '=',
    '>',
    '<',
    '!',
    0
};

const string doubleSymbol[] = {
    "==",
    ">=",
    "<=",
    "!=" ,
    ""
};

struct token{
    string type;
    string value;
};

STATE findNext(STATE curState, char input, string &buffer, vector<token> &anaRes);
void operation(STATE curState, string &buffer, vector<token> &anaRes);
STATE letterCase(STATE curState, char input, string &buffer, vector<token> &anaRes);
STATE numberCase(STATE curState, char input, string &buffer, vector<token> &anaRes);
STATE symbolCase(STATE curState, char input, string &buffer, vector<token> &anaRes);
