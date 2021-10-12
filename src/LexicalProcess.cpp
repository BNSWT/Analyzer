/*
 * Created Date: 2021-10-12 21:04:04
 * Author: yuyangz
 */

#include <iostream>
#include <string>
#include <vector>

#include "Lexical.h"
#include "LexicalDFA.cpp"

using namespace std;

string deFormat(string source)
{
    return "";
}

vector<string> mainProcess(string source)
{
    string deFormatted = deFormat(source);
    STATE state = STATE::INIT;

    vector<string> res;
    for (int i = 0; i < source.size(); i++){
        state = findNext(state, source[i], res);
    }
    return res;
}