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

using namespace std;

enum STATE{
    INIT, //Init state.
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

const string assign[] = {
    "=",
    "+=",
    "-=",
    "*=",
    "/="
    ""
};

const string calculator[]={
    //calculate
    "+",
    "-",
    "*",
    "/",
    "++",
    "--",
    //compare
    ">",
    "<",
    ">=",
    "<=",
    "==",
    "!=",
    //logic
    "&&",
    "||",
    "!",
    // bit calculate
    "~",
    "&",
    "|",
    "<<",
    ">>",

    ""
};

const string delimeter[] = {
    ";",
    ""
};

const string divider[]={
    ","
    ""
};

const string bracket[]={
    "(",
    ")",
    "{",
    "}"
    "",
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
    '{',
    '}',
    '(',
    ')',
    ',',
    ';',
    0
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

vector<token> mainProcess(string source);
void fileProcess(const char* inputPath, const char* outputPath);