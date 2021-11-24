/*
 * Created Date: 2021-11-17 09:20:13
 * Author: yuyangz
 */


#include"Parser.h"

extern void generateLRTable(vector<projectSet> &DFAstates,vector<vector<int>> &GOTO, vector<vector<actionElem>> &ACTION);

extern pair<SYNTAX_STATE, vector<rightElem>> findFormula(int cnt);


analyzeTree makeTree(multimap<rightElem, vector<rightElem>> treeInfo)
{
    analyzeTree t = NULL;
    return t;
}

string wrongHint(token term)
{
    return  "the" + names[term.type] + " " + term.value + "is not recognized";
}

void mainProcess(vector<token> inputString, vector<vector<int>> gotoTable, vector<vector<actionElem>> actionTable, vector<analyzeProcess> &parseProcess, analyzeTree &parseTree, string &errlog)
{
    int curState = 0;
    multimap<rightElem, vector<rightElem>> treeInfo;
    while(inputString.size()) {
        int termID = inputString[inputString.size()-1].type;
        inputString.pop_back();
        actionElem aElem = actionTable[curState][termID];
        stack<int> stateStack = parseProcess[parseProcess.size()-1].stateStack;
        stack<rightElem> elemStack = parseProcess[parseProcess.size()-1].symbolStack;   
        switch(aElem.gotoElemType) {
            case GOTO_ELEM_TYPE::MOVE:
                stateStack.push(aElem.des);
                elemStack.push({RIGHT_ELEM_TYPE::TERMINATER,termID});
                parseProcess.push_back({stateStack, elemStack, inputString});
                break;
            case GOTO_ELEM_TYPE::INDUCE:{
                auto formula = findFormula(aElem.des);
                vector<rightElem> children;
                for (int i =0; i < formula.second.size(); i++) {
                    children.push_back(elemStack.top());
                    stateStack.pop();
                    elemStack.pop();
                }
                auto left = formula.first;
                auto nextState = gotoTable[curState][formula.first];
                rightElem nextRightElem{RIGHT_ELEM_TYPE::STATE, left};
                stateStack.push(nextState);
                elemStack.push(nextRightElem);
                parseProcess.push_back({stateStack, elemStack, inputString});
                //treeInfo.insert({nextRightElem, children});
                break;
            }
            default:
                errlog = wrongHint(inputString[inputString.size()-1]);
                break;
        }
        if (errlog.size())
            break;
    }
}

void parserProcess(vector<token> inputString, vector<analyzeProcess> &parseProcess, analyzeTree &parseTree, string &errlog)
{
    vector<projectSet> DFAstates;
    vector<vector<int>> gotoTable;
    vector<vector<actionElem>> actionTable;
    generateLRTable(DFAstates, gotoTable, actionTable);
    mainProcess(inputString, gotoTable, actionTable, parseProcess, parseTree, errlog);
}