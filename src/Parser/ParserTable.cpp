/*
 * Created Date: 2021-11-17 09:22:27
 * Author: yuyangz
 */

#include "Parser.h"

vector<vector<project>> parseProjects()
{

}

vector<project> closure(project I)
{

}

vector<project> gotoFuc(vector<project> cur, rightElem input)
{

}

vector<vector<project>> generateDFA()
{

}

void generateTable(vector<vector<project>> &DFAstates,vector<vector<int>> &gotoTable, vector<vector<actionElem>> &actionTable)
{

}

void generateLRTable(vector<vector<project>> &DFAstates,vector<vector<int>> &gotoTable, vector<vector<actionElem>> &actionTable)
{
    DFAstates=generateDFA();
    generateTable(DFAstates, gotoTable, actionTable);
    
}