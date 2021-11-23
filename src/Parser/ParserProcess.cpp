/*
 * Created Date: 2021-11-17 09:20:13
 * Author: yuyangz
 */


#include"Parser.h"

extern void generateLRTable(vector<projectSet> &DFAstates,vector<vector<int>> &GOTO, vector<vector<actionElem>> &ACTION);

extern pair<SYNTAX_STATE, vector<rightElem>> findFormula(int cnt);

void updateProcess()
{

}

void updateTree()
{

}

void wrongHint()
{

}

void mainProcess(vector<enum TYPE> inputString, vector<vector<int>> gotoTable, vector<vector<actionElem>> actionTable, analyzeProcess &parseProcess, analyzeTree &parseTree)
{
    int curState = 0;
    stack<int> stateStack;
    stack<rightElem> elemStack;
    for (auto term = inputString.begin(); term < inputString.end(); term++) {
        int termID = *term;
        actionElem aElem = actionTable[curState][termID];
        switch(aElem.gotoElemType) {
            case GOTO_ELEM_TYPE::MOVE:
                stateStack.push(aElem.des);
                elemStack.push({RIGHT_ELEM_TYPE::TERMINATER,termID});
                updateProcess();
                updateTree();
                break;
            case GOTO_ELEM_TYPE::INDUCE:{
                auto formula = findFormula(aElem.des);
                for (int i =0; i < formula.second.size(); i++) {
                    stateStack.pop();
                    elemStack.pop();
                }
                auto left = formula.first;
                auto nextState = gotoTable[curState][formula.first];
                stateStack.push(nextState);
                elemStack.push({RIGHT_ELEM_TYPE::STATE, left});
                updateProcess();
                updateTree();
                break;
            }
            default:
                wrongHint();
                break;
        }
    }
}

void parserProcess(vector<enum TYPE> inputString, analyzeProcess &parseProcess, analyzeTree parseTree)
{
    vector<projectSet> DFAstates;
    vector<vector<int>> gotoTable;
    vector<vector<actionElem>> actionTable;
    generateLRTable(DFAstates, gotoTable, actionTable);
    mainProcess(inputString, gotoTable, actionTable, parseProcess, parseTree);
}