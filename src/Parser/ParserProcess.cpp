/*
 * Created Date: 2021-11-17 09:20:13
 * Author: yuyangz
 */


#include"Parser.h"

extern void generateLRTable(vector<vector<project>> &DFAstates,vector<vector<int>> &GOTO, vector<vector<actionElem>> &ACTION);

void mainProcess(vector<enum TYPE> inputString, vector<vector<int>> gotoTable, vector<vector<actionElem>> actionTable, analyzeProcess &parseProcess, analyzeTree parseTree)
{
    
}

void parserProcess(vector<enum TYPE> inputString, analyzeProcess &parseProcess, analyzeTree parseTree)
{
    vector<vector<project>> DFAstates;
    vector<vector<int>> gotoTable;
    vector<vector<actionElem>> actionTable;
    generateLRTable(DFAstates, gotoTable, actionTable);
    mainProcess(inputString, gotoTable, actionTable, parseProcess, parseTree);
}