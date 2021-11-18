#pragma once
/*
 * Created Date: 2021-10-12 21:04:13
 * Author: yuyangz
 */

#include <string>
#include <vector>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;

enum STATE{
    INIT, //Init state.
    LETTER,
    NUM,
    SYMBOL,
    OPEN_DOT,
    OPEN_QUOTE,
    FLOAT,
    STRING
};

enum TYPE{
    HEAD,//also means epsilon
    WELL,
    INCLUDE,
    CHAR,
    TYPE,
    MAIN,
    DIVIDER,
    DELIMETER,
    L_BIG_BRACKET,
    R_BIG_BRACKET,
    L_MID_BRACKET,
    R_MID_BRACKET,
    L_SMA_BRACKET,
    R_SMA_BRACKET,
    L_SHA_BRACKET,
    R_SHA_BRACKET,
    ADDSUB,
    PLUSDIV,
    COMP,
    EXCLAINMATION,
    LOGIC,
    WAVE,
    BIT,
    IF,
    ELSE,
    WHILE,
    RETURN,
    ASSIGN,
    CAL_ASSIGN,
    SELF,
    INTEGER,
    FLOAT_VALUE,
    CHARACTER,
    STRING_VALUE,
    ID,
    UNDEFINED,
    TAIL
};

enum INPUT_TYPE{
    LETTER_INPUT,
    NUMBER_INPUT,
    SYMBOL_INPUT,
    DOT_INPUT,
    QUOTE_INPUT
};

enum SYMBOL_TYPE{
    MUST_SINGLE,
    MAY_DOUBLE,
    NOT_INCLUDED
};

const string dicts[][10] = {
    //HEAD,
    {},
    //WELL,
    {
        "#",
        ""
    },
    //INCLUDE,
    {
        "include",
        ""
    },
    //CHAR,
    {
        "char",
        ""
    },
    //TYPE,
    {
        "float",
        "double",
        "int",
        ""
    },
    //MAIN,
    {
        "main",
        ""
    },
    //DIVIDER,
    {
        ",",
        ""
    },
    //DELIMETER,
    {
        ";",
        ""
    },
    //L_BIG_BRACKET,
    {
        "{",
        ""
    },
    //R_BIG_BRACKET,
    {
        "}",
        ""
    },
    //L_MID_BRACKET,
    {
        "[",
        ""
    },
    //R_MID_BRACKET,
    {
        "]",
        ""
    },
    //L_SMA_BRACKET,
    {
        "(",
        ""
    },
    //R_SMA_BRACKET,
    {
        ")",
        ""
    },
    //L_SHA_BRACKET,
    {
        "<",
        ""
    },
    //R_SHA_BRACKET,
    {
        ">",
        ""
    },
    //ADDSUB,
    {
        "+",
        "-",
        ""
    },
    //PLUSDIV,
    {
        "*",
        "/",
        ""
    },
    //COMP,
    {
        ">",
        "<",
        ">=",
        "<=",
        "==",
        "!=",
        ""
    },
    //EXCLAINMATION,
    {
        "!",
        ""
    },
    //LOGIC,
    {
        "&&",
        "||",
        ""
    },
    //WAVE,
    {
        "~",
        ""
    },
    //BIT,
    {
        "&",
        "|",
        "<<"
        ">>",
        ""
    },
    //IF,
    {
        "if",
        ""
    },
    //ELSE,
    {
        "else",
        ""
    },
    //WHILE,
    {
        "while",
        ""
    },
    //RETURN,
    {
        "return",
        ""
    },
    //"ASSIGN",
    {
        "=",
        ""
    },
    //"CAL_ASSIGN",
    {
        "+=",
        "-=",
        "*=",
        "/=",
        ""
    },
    //"SELF"
    {
        "++",
        "--",
        ""
    }
};

const string names[]={
    "",
    "WELL",
    "INCLUDE",
    "CHAR",
    "TYPE",
    "MAIN",
    "DIVIDER",
    "DELIMETER",
    "L_BIG_BRACKET",
    "R_BIG_BRACKET",
    "L_MID_BRACKET",
    "R_MID_BRACKET",
    "L_SMA_BRACKET",
    "R_SMA_BRACKET",
    "L_SHA_BRACKET",
    "R_SHA_BRACKET",
    "ADDSUB",
    "PLUSDIV",
    "COMP",
    "EXCLAINMATION",
    "LOGIC",
    "WAVE",
    "BIT",
    "IF",
    "ELSE",
    "WHILE",
    "RETURN",
    "ASSIGN",
    "CAL_ASSIGN",
    "SELF",
    "INTEGER",
    "FLOAT",
    "CHARACTER",
    "STRING",
    "ID",
    "UNDEFINED",
    "TAIL"
    ""
};

const char mustSingleSymbol[] = {
    '{',
    '}',
    '(',
    ')',
    '<',
    '>',
    '[',
    ']',
    ',',
    ';',
    '~'
};

const char mayDoubleSymbol[] = {
    '+',
    '-',
    '*',
    '/',
    '=',
    '>',
    '<',
    '!',
    '&',
    '|',
};

const string doubleSymbol[] = {
    "==",
    ">=",
    "<=",
    "!=",
    "+=",
    "-=",
    "*=",
    "/=",
    ""
};

struct token{
    enum TYPE type;
    string value;
};

vector<token> lexicalProcess(string source);
void fileProcess(const char* inputPath, const char* outputPath);
