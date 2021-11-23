/*
 * Created Date: 2021-11-17 09:22:27
 * Author: yuyangz
 */

#include "Parser.h"

projectSet allProjects()
{   
    projectSet set;
    for (SYNTAX_STATE synState = SYNTAX_STATE::SBEG; synState <= SYNTAX_STATE::V; synState=(SYNTAX_STATE)(synState+1)) {
        auto pairIter = grammar.equal_range(synState);
        if (pairIter.first != grammar.end()) {
            for (auto iter = pairIter.first; iter != pairIter.second; iter++) {
                vector<rightElem> formula= iter->second;
                for (int i=0; i<=formula.size(); i++) {
                    projectRight pRight{formula, i};
                    set.insert({synState, pRight});
                }
            }
        }
    }
    return set;
}

projectSet formula2projects(pair<SYNTAX_STATE, vector<rightElem>> formula)
{
    projectSet set;
    for (int i = 0; i < formula.second.size(); i++) {
        projectRight pRight{formula.second, i};
        set.insert({formula.first, pRight});
    }
    return set;
}

projectSet closure(projectSet I, projectSet allProj)
{
    projectSet clos=I;// deep copy
    projectSet newClos = clos;
    do{
        clos = newClos;
        for (auto iter = clos.begin(); iter != clos.end(); iter++) {
            int dotpos = iter->second.dotpos;
            vector<rightElem> right = iter->second.right;
            if (dotpos < right.size() && right[dotpos].type==RIGHT_ELEM_TYPE::STATE) {
                SYNTAX_STATE synState = (SYNTAX_STATE)right[dotpos].index;
                auto pairIter = allProj.equal_range(synState);
                if (pairIter.first != allProj.end()){
                    for (auto chosenProj=pairIter.first; chosenProj != pairIter.second; chosenProj++) {
                        if (chosenProj->second.dotpos==0){
                            newClos.insert(*chosenProj);
                        }
                    }
                } 
            }
        }
    }while(newClos.size()!=clos.size());
}

projectSet gotoFuc(projectSet cur, rightElem input, projectSet allProj)
{
    projectSet dest;
    for(auto iter=cur.begin(); iter != cur.end(); iter++) {
        int dotpos = iter->second.dotpos;
        vector<rightElem> right = iter->second.right;
        if (dotpos < right.size() && right[dotpos].type==input.type && right[dotpos].index == input.index) {
            projectRight pRight{right, dotpos+1};
            dest.insert({iter->first, pRight});
        }
    }
    return dest;
}

vector<projectSet> generateDFA(projectSet allProj)
{
    vector<projectSet> DFA, newDFA;
    projectSet init=
    {
        {
            SYNTAX_STATE::SBEG,
            {
                {
                    {RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::BEG}
                },
                0
            }
        }
    };
    newDFA.push_back(closure(init, allProj));
    do {
        DFA=newDFA;
        for (auto iter=DFA.begin(); iter!=DFA.end(); iter++) {
            for (SYNTAX_STATE synState = SYNTAX_STATE::SBEG; synState <= SYNTAX_STATE::V; synState=(SYNTAX_STATE)(synState+1)) {
                auto nextSet = gotoFuc(*iter, {RIGHT_ELEM_TYPE::STATE,synState}, allProj);
                if (nextSet.size()) {
                    newDFA.push_back(nextSet);
                }
            }
        }
    }while(newDFA.size()!=DFA.size());
}

void initTable(vector<projectSet> &DFAstates, vector<vector<int>> &gotoTable, vector<vector<actionElem>> &actionTable)
{
    for (int i =0 ; i<DFAstates.size(); i++){
        vector<int> gotoRow;
        for (enum TYPE type=TYPE::HEAD; type < TYPE::TAIL; type = (enum TYPE)(type+1)) {
            gotoRow.push_back(-1);
        }
        gotoTable.push_back(gotoRow);
        vector<actionElem> actionRow;
        for (SYNTAX_STATE synState = SYNTAX_STATE::SBEG; synState <= SYNTAX_STATE::V; synState=(SYNTAX_STATE)(synState+1)) {
            actionRow.push_back({GOTO_ELEM_TYPE::ERR, -1});
        }
        actionTable.push_back(actionRow);
    } 
}

int locateDFAstate(vector<projectSet> DFAstates, projectSet I)
{
    auto stateIter=DFAstates.begin();
    bool found = false;
    for (; stateIter!=DFAstates.end(); stateIter++) {
        auto iter1=stateIter->begin();
        auto iter2=I.begin();
        found = true;
        for (; iter1 !=stateIter->end() && iter2 !=I.end(); iter1++, iter2++) {
            if (iter1->first!=iter2->first ||
                iter1->second.dotpos!=iter2->second.dotpos) {
                found = false;
                break;
            }
            auto subIter1 = iter1->second.right.begin();
            auto subIter2 = iter2->second.right.begin();
            for (; subIter1<iter1->second.right.end() && subIter2<iter2->second.right.end(); subIter1++, subIter2++) {
                if (*subIter1!=*subIter2) {
                    found = false;
                    break;
                }
            }
        }
        if (found == true)
            break;
    }
    return stateIter-DFAstates.begin();
}

int locateFormula(pair<SYNTAX_STATE, vector<rightElem>> formula)
{
    int cnt = 0;
    for (auto iter=grammar.begin(); iter != grammar.end(); iter++) {
        cnt++;
        if (iter->first==formula.first && iter->second==formula.second) {
            break;
        }
    }
    return cnt;
}

pair<SYNTAX_STATE, vector<rightElem>> findFormula(int cnt)
{
    auto iter=grammar.begin();
    for (int i = 0; i < cnt && iter != grammar.end(); iter++, i++)
    ;
    return *iter;
}

void generateTable(vector<projectSet> &DFAstates,vector<vector<int>> &gotoTable, vector<vector<actionElem>> &actionTable, projectSet allProj)
{
    initTable(DFAstates, gotoTable, actionTable);
    for (auto DFAstate=DFAstates.begin(); DFAstate!=DFAstates.end(); DFAstate++) {
        for (auto iter=DFAstate->begin(); iter !=DFAstate->end(); iter++) {
            int dotpos = iter->second.dotpos;
            vector<rightElem> right = iter->second.right;
            int curStateNum = DFAstate-DFAstates.begin();
            rightElem input = right[dotpos];
            if(dotpos==right.size()) {
                int nextFormulaNum = locateFormula({iter->first, iter->second.right});
                actionTable[curStateNum][input.index].gotoElemType = GOTO_ELEM_TYPE::INDUCE;
                actionTable[curStateNum][input.index].des=nextFormulaNum;
            }
            else if (right[dotpos].type==RIGHT_ELEM_TYPE::TERMINATER) {
                int nextStateNum = locateDFAstate(DFAstates,gotoFuc(*DFAstate, input, allProj));
                actionTable[curStateNum][input.index].gotoElemType=GOTO_ELEM_TYPE::MOVE;
                actionTable[curStateNum][input.index].des=nextStateNum;
            }
        }
    }
}

void generateLRTable(vector<projectSet> &DFAstates,vector<vector<int>> &gotoTable, vector<vector<actionElem>> &actionTable)
{
    projectSet allProj = allProjects();
    DFAstates=generateDFA(allProj);
    generateTable(DFAstates, gotoTable, actionTable, allProj);
}