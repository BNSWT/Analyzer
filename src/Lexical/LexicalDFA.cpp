/*
 * Created Date: 2021-10-12 21:29:25
 * Author: yuyangz
 */


#include"Lexical.h"

using namespace std;

STATE findNext(STATE curState, char input, string &buffer, vector<token> &anaRes);
void operation(STATE curState, string &buffer, vector<token> &anaRes);
static STATE letterCase(STATE curState, char input, string &buffer, vector<token> &anaRes);
static STATE numberCase(STATE curState, char input, string &buffer, vector<token> &anaRes);
static STATE symbolCase(STATE curState, char input, string &buffer, vector<token> &anaRes);

static INPUT_TYPE findInputType(char input)
{
    if ((input >= 'a' && input <= 'z') || (input >= 'A' && input <='Z'))
        return INPUT_TYPE::LETTER_INPUT;
    else if (input >= '0' && input <= '9')
        return INPUT_TYPE::NUMBER_INPUT;
    else if (input == '.')
        return INPUT_TYPE::DOT_INPUT;
    else if (input == '"')
        return INPUT_TYPE::QUOTE_INPUT;
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

static bool lookUp(string &item, vector<token> &anaRes)
{
    for (int type = TYPE::HEAD; type < TYPE::TAIL; type++){
        const string *dict = dicts[type];
        const string dictName = names[type];
        for (int i = 0; dict[i]!=""; i++) {
            if (item==dict[i]) {
                anaRes.push_back({type, item});
                item.clear();
                return true;
            }
        }
    }
    return false;
}

void operation(STATE curState, string &buffer, vector<token> &anaRes)
{
    if (buffer.size()){
        switch(curState) {
            case STATE::LETTER:
                if(lookUp(buffer, anaRes))
                    return;
                for (int i =0; i < (int)buffer.size(); i++) {
                    if (findInputType(buffer[i])==INPUT_TYPE::SYMBOL_INPUT) {
                        anaRes.push_back({TYPE::UNDEFINED, buffer});
                        buffer.clear();
                        return;
                    }
                }
                anaRes.push_back({TYPE::ID, buffer});
                break;
            case STATE::NUM:
                anaRes.push_back({TYPE::INTEGER, buffer});
                break;
            case STATE::SYMBOL:
                if(lookUp(buffer, anaRes))
                    return;
                anaRes.push_back({TYPE::UNDEFINED, buffer});
                break;
            case STATE::OPEN_DOT:
                anaRes.push_back({TYPE::UNDEFINED, buffer});
                break;
            case STATE::OPEN_QUOTE:
                anaRes.push_back({TYPE::UNDEFINED, buffer});
                break;
            case STATE::FLOAT:
                anaRes.push_back({TYPE::FLOAT, buffer});
                break;
            default:
                anaRes.push_back({TYPE::UNDEFINED, buffer});
                break;
        }
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
        case INPUT_TYPE::NUMBER_INPUT:
            buffer += input;
            return STATE::LETTER;
        default:
            operation(curState, buffer, anaRes);
            return findNext(STATE::INIT, input, buffer, anaRes);
    }
}

static STATE numberCase(STATE curState, char input, string &buffer, vector<token> &anaRes)
{
    //number or wrong.
    switch(findInputType(input)) {
        case INPUT_TYPE::NUMBER_INPUT:
            buffer += input;
            return STATE::NUM;
        case INPUT_TYPE::DOT_INPUT:
            buffer += input;
            return STATE::OPEN_DOT;
        default:
            operation(curState, buffer, anaRes);
            return findNext(STATE::INIT, input, buffer, anaRes);
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
            operation(curState, buffer, anaRes);
            return findNext(STATE::INIT, input, buffer, anaRes);
            break;
    }
}

static STATE openDotCase(STATE curState, char input, string &buffer, vector<token> &anaRes)
{
    switch(findInputType(input)) {
        case INPUT_TYPE::NUMBER_INPUT:
            buffer += input;
            return STATE::FLOAT;
        default:
            operation(curState, buffer, anaRes);
            return findNext(STATE::INIT, input, buffer, anaRes);
    }
}

static STATE openQuoteCase(STATE curState, char input, string &buffer, vector<token> &anaRes)
{
    switch(findInputType(input)) {
        case INPUT_TYPE::QUOTE_INPUT:
            buffer += input;
            operation(curState, buffer, anaRes);
            return STATE::INIT;
        default:
            buffer += input;
            return STATE::OPEN_QUOTE;
    }
}

static STATE floatCase(STATE curState, char input, string &buffer, vector<token> &anaRes)
{
    switch(findInputType(input)) {
        case INPUT_TYPE::NUMBER_INPUT:
            buffer += input;
            return STATE::FLOAT;
            break;
        default:
            operation(curState, buffer, anaRes);
            return findNext(STATE::INIT, input, buffer, anaRes);
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
            case NUMBER_INPUT:
                return STATE::NUM;
            case QUOTE_INPUT:
                return STATE::OPEN_QUOTE;
            default:
                return STATE::SYMBOL;
        }
    }
    else {
        switch(curState) {
            case STATE::LETTER:
                return letterCase(curState, input, buffer, anaRes);
            case STATE::NUM:
                return numberCase(curState, input, buffer, anaRes);
            case STATE::SYMBOL:
                return symbolCase(curState, input, buffer, anaRes);
            case STATE::OPEN_DOT:
                return openDotCase(curState, input, buffer, anaRes);
            case STATE::OPEN_QUOTE:
                return openQuoteCase(curState, input, buffer, anaRes);
            case STATE::FLOAT:
                return floatCase(curState, input, buffer, anaRes);
            default:
                return STATE::INIT;
                break;
        }
    }
}