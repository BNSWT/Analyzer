/*
 * Created Date: 2021-10-12 21:29:25
 * Author: yuyangz
 */
#include <string>
#include <vector>
#include <cstring>

#include"Lexical.h"

using namespace std;

INPUT_TYPE findInputType(char input)
{
    if (input >= 'a' && input <= 'z' || input >= 'A' && input <='Z')
        return INPUT_TYPE::LETTER_INPUT;
    else if (input >= '0' && input <= '9')
        return INPUT_TYPE::NUMBER_INPUT;
    else
        return INPUT_TYPE::SYMBOL_INPUT;
}

SYMBOL_TYPE findSymbolType(char input)
{
    for (int i =0; mustSingleSymbol[i]!=0; i++) {
        if (input == mustSingleSymbol[i])
            return  SYMBOL_TYPE::MUST_SINGLE;
    }
    for (int i = 0; mayDoubleSymbol[i]!=0; i++) {
        if (input == mayDoubleSymbol[i])
            return SYMBOL_TYPE::MAY_DOUBLE;
    }
    return SYMBOL_TYPE::NOT_INCLUDED;
}

void operation(STATE curState, string &buffer, vector<token> &anaRes)
{
    switch(curState) {
        case STATE::LETTER:
            for (int i = 0; reservedWords[i]!=""; i++) {
                if (buffer == reservedWords[i])
                    anaRes.push_back({"reserved word", buffer});
                    buffer.clear();
                    return;
            }
            anaRes.push_back({"identifier", buffer});
            break;
        case STATE::NUM:
            anaRes.push_back({"number", buffer});
            break;
        case STATE::SYMBOL:
            anaRes.push_back({"symbol", buffer});
    }
    buffer.clear();
} 

STATE letterCase(STATE curState, char input, string &buffer, vector<token> &anaRes)
{
    // identifier or reserved word. Judged in operation.
    switch(findInputType(input)) {
        case INPUT_TYPE::LETTER_INPUT:
            buffer += input;
            return STATE::LETTER;
            break;
        case INPUT_TYPE::NUMBER_INPUT:
            buffer += input;
            return STATE::LETTER;
            break;
        default:
            operation(curState, buffer, anaRes);
            curState = STATE::INIT;
            return symbolCase(curState, input, buffer, anaRes);
            break;
    }
}

STATE numberCase(STATE curState, char input, string &buffer, vector<token> &anaRes)
{
    //number or wrong.
    switch(findInputType(input)) {
        case INPUT_TYPE::NUMBER_INPUT:
            return STATE::NUM;
            break;
        default:
            operation(curState, buffer, anaRes);
            curState = STATE::SYMBOL;
            return symbolCase(curState, input, buffer, anaRes);
            break;
    }
}

STATE symbolCase(STATE curState, char input, string &buffer, vector<token> &anaRes)
{
    switch(findSymbolType(input)) {
        case SYMBOL_TYPE::MUST_SINGLE:
            if (buffer.size()) 
                operation(curState, buffer, anaRes);
            buffer = input;
            operation(curState, buffer, anaRes);
            return STATE::INIT;
            break;
        case SYMBOL_TYPE::MAY_DOUBLE:
            buffer += input;
            return STATE::SYMBOL;
            break;
        default:
            if (buffer.size()) {
                operation(curState, buffer, anaRes);
                curState = STATE::INIT;
            }
            return findNext(curState, input, buffer, anaRes);
            break;
    }
}

STATE findNext(STATE curState, char input, string &buffer, vector<token> &anaRes)
{
    if (input == ' ') {
        operation(curState, buffer, anaRes);
        return STATE::INIT;
    }
    if (curState == STATE::INIT) {
        buffer += input;
        switch(findInputType(input)) {
            case LETTER_INPUT:
                return STATE::LETTER;
                break;
            case NUMBER_INPUT:
                return STATE::NUM;
                break;
            case SYMBOL_INPUT:
                return STATE::SYMBOL;
                break;
            default:
                return STATE::INIT;
                break;
        }
    }
    else {
        switch(curState) {
            case STATE::LETTER:
                return letterCase(curState, input, buffer, anaRes);
                break;
            case STATE::NUM:
                return numberCase(curState, input, buffer, anaRes);
                break;
            case STATE::SYMBOL:
                return symbolCase(curState, input, buffer, anaRes);
                break;
            default:
                return STATE::INIT;
                break;
        }
    }
}
