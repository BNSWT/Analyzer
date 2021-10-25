/*
 * Created Date: 2021-10-12 21:29:25
 * Author: yuyangz
 */


#include"Lexical.h"

using namespace std;

STATE findNext(STATE curState, char input, string &buffer, vector<token> &anaRes);
static void operation(STATE curState, string &buffer, vector<token> &anaRes);
static STATE letterCase(STATE curState, char input, string &buffer, vector<token> &anaRes);
static STATE numberCase(STATE curState, char input, string &buffer, vector<token> &anaRes);
static STATE symbolCase(STATE curState, char input, string &buffer, vector<token> &anaRes);

static INPUT_TYPE findInputType(char input)
{
    if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <='Z'))
        return INPUT_TYPE::LETTER_INPUT;
    else if (input >= '0' && input <= '9')
        return INPUT_TYPE::NUMBER_INPUT;
    else
        return INPUT_TYPE::SYMBOL_INPUT;
}

static SYMBOL_TYPE findSymbolType(char input)
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

static bool lookUp(const string dict[], const char* dictName, string &item, vector<token> &anaRes)
{
    for (int i = 0; dict[i]!=""; i++) {
        if (item==dict[i]) {
            anaRes.push_back({dictName, item});
            item.clear();
            return true;
        }
    }
    return false;
}

static void operation(STATE curState, string &buffer, vector<token> &anaRes)
{
    switch(curState) {
        case STATE::LETTER:
            if(lookUp(reservedWords, "reserved word",buffer, anaRes))
                return;
            for (int i =0; i < (int)buffer.size(); i++) {
                if (findInputType(buffer[i])==INPUT_TYPE::SYMBOL_INPUT) {
                    anaRes.push_back({"undefined", buffer});
                    buffer.clear();
                    return;
                }
            }
            anaRes.push_back({"identifier", buffer});
            break;
        case STATE::NUM:
            anaRes.push_back({"number", buffer});
            break;
        case STATE::SYMBOL:
            if(lookUp(assign, "assign symbol",buffer, anaRes))
                return;
            if(lookUp(calculator, "calculator symbol",buffer, anaRes))
                return;
            if(lookUp(delimeter, "delimeter symbol",buffer, anaRes))
                return;
            if(lookUp(divider, "divider symbol",buffer, anaRes))
                return;
            if(lookUp(bracket, "bracket symbol",buffer, anaRes))
                return;
            anaRes.push_back({"undefined", buffer});
            break;
        default:
            break;
    }
    buffer.clear();
} 

static STATE letterCase(STATE curState, char input, string &buffer, vector<token> &anaRes)
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
            curState = STATE::SYMBOL;
            return symbolCase(curState, input, buffer, anaRes);
            break;
    }
}

static STATE numberCase(STATE curState, char input, string &buffer, vector<token> &anaRes)
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

static STATE symbolCase(STATE curState, char input, string &buffer, vector<token> &anaRes)
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
            if (buffer.size() == 0) {
                buffer += input;
            }
            operation(curState, buffer, anaRes);
            curState = STATE::INIT;
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
