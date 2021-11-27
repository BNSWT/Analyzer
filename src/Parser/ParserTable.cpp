/*
 * Created Date: 2021-11-17 09:22:27
 * Author: yuyangz
 */

#include "Parser.h"
#include <algorithm>
static projectSet allProj;
static map<int, vector<enum TYPE>> firstSets;
static map<int, vector<enum TYPE>> followSets;

int gotoColNum(RIGHT_ELEM_TYPE type,int index)
{   
    if (type == RIGHT_ELEM_TYPE::STATE)
        return index;
    else
        return index+20;
}

int gotoColNum(enum TYPE index)
{
    return index+20;
}

int gotoColNum(SYNTAX_STATE index)
{
    return index;
}

int gotoColNum(rightElem elem)
{
    return gotoColNum(elem.type, elem.index);
}

bool hasEmpty(rightElem elem)
{
    auto range = grammar.equal_range((SYNTAX_STATE)elem.index);
    if (range.first!=grammar.end()) {
        for (auto iter=range.first; iter!=range.second; iter++) {
            if (iter->second.size()==0)
                return true;
        }
    }
    return false;
}

static inline void generateFirst()
{
    //1.
    for (enum TYPE type=TYPE::HEAD; type < TYPE::TAIL; type = (enum TYPE)(type+1)) {
        firstSets.insert({gotoColNum(type), {(enum TYPE)type}});
    }
    for (SYNTAX_STATE synState = SYNTAX_STATE::SBEG; synState <= SYNTAX_STATE::V; synState=(SYNTAX_STATE)(synState+1)) {
        firstSets.insert({gotoColNum(synState),{}});
    }
    auto newFirst=firstSets;
    do{
        firstSets=newFirst;
        for(auto iter=grammar.begin(); iter!=grammar.end(); iter++) {
            //2.
            auto left = iter->first;
            int leftIndex = gotoColNum(RIGHT_ELEM_TYPE::STATE,left);
            if (iter->second.size()==0)
                continue;
            auto rightIter = iter->second.begin();
            int mapIndex=gotoColNum(rightIter->type, rightIter->index);
            do{
                for (auto fir=firstSets[mapIndex].begin(); fir!=firstSets[mapIndex].end();fir++) {
                    // cout << *fir << endl;
                    newFirst[leftIndex].push_back(*fir);
                }
                rightIter++;
            }while(hasEmpty(*rightIter));
            //3.
            if (iter->second.size()==0)
                newFirst[leftIndex].push_back(TYPE::HEAD);
        }
    }while(firstSets.size()!=newFirst.size());
}

static inline void generateFollow()
{
    // initialize
    for (enum TYPE type=TYPE::HEAD; type < TYPE::TAIL; type = (enum TYPE)(type+1)) {
        followSets.insert({gotoColNum(type), {}});
    }
    for (SYNTAX_STATE synState = SYNTAX_STATE::SBEG; synState <= SYNTAX_STATE::V; synState=(SYNTAX_STATE)(synState+1)) {
        followSets.insert({gotoColNum(synState),{}});
    }
    auto newFollow=followSets;
    do{
        followSets = newFollow;
        for (auto iter = grammar.begin(); iter!=grammar.end(); iter++) {
            //2
            for (auto rightIter = iter->second.begin()+1; rightIter<iter->second.end(); rightIter++) {
                int nextIndex = gotoColNum(*(rightIter+1));
                for (auto firIter = firstSets[nextIndex].begin(); firIter!=firstSets[nextIndex].end(); firIter++) {
                    if (*firIter!=TYPE::HEAD)
                        newFollow[gotoColNum(*rightIter)].push_back(*firIter);
                }
            }
            //3
            if (iter->second.size()==0)
                continue;
            auto tail = iter->second.end()-1;
            if (find(firstSets[gotoColNum(*tail)].begin(), firstSets[gotoColNum(*tail)].end(), TYPE::HEAD)!=firstSets[gotoColNum(*tail)].end()){
                for (auto lIter = firstSets[gotoColNum(iter->first)].begin(); lIter !=firstSets[gotoColNum(iter->first)].end(); lIter++) {
                    newFollow[gotoColNum(*tail)].push_back(*lIter);
                }
            }
        }
    }while(newFollow.size()!=followSets.size());
}

static inline void getAllProjects()
{
    for (SYNTAX_STATE synState = SYNTAX_STATE::SBEG; synState <= SYNTAX_STATE::V; synState=(SYNTAX_STATE)(synState+1)) {
        auto pairIter = grammar.equal_range(synState);
        if (pairIter.first != grammar.end()) {
            for (auto iter = pairIter.first; iter != pairIter.second; iter++) {
                vector<rightElem> formula= iter->second;
                for (int i=0; i<=formula.size(); i++) {
                    for (auto follow = followSets[gotoColNum(synState)].begin(); follow != followSets[gotoColNum(synState)].end(); follow++) {
                        projectRight pRight{formula, i, *follow};
                        allProj.insert({synState, pRight});
                    }
                }
            }
        }
    }
}

static inline bool in(projectSet &child, projectSet &parent)
{
    for (auto childIter=child.begin(); childIter !=child.end(); childIter++) {
        // ensure the pair can be found in parent.
        auto childKey = childIter->first;
        auto childValue = childIter->second;

        // grab all pairs with the same key.
        auto matchedPairs = parent.equal_range(childKey);

        // find the pair in this range.
        bool found = false;
        if(matchedPairs.first!=matchedPairs.second) {
            for (auto pairIter=matchedPairs.first; pairIter != matchedPairs.second; pairIter++) {
                // check if the two pairs truly equal.
                found = true;
                if(pairIter->first!=childKey) {
                    found = false;
                    continue;
                }
                if (pairIter->second.dotpos!=childValue.dotpos){
                    found = false;
                    continue;
                }
                if (pairIter->second.look!=childValue.look){
                    found = false;
                    continue;
                }
                auto value=pairIter->second.right;
                auto cIter=childValue.right.begin();
                auto pIter=value.begin();
                for (; cIter != childValue.right.end()&&pIter!=value.end(); cIter++, pIter++) {
                    if (cIter->index!=pIter->index || cIter->type!=pIter->type){
                        found = false;
                        break;
                    }
                }
                if (found)
                    break; // check next pair.
            }
        }
        else
            return false;
    }
    return true;
}

static inline projectSet closure(projectSet I)
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
                //all projects begins with synState.
                auto pairIter = allProj.equal_range(synState);
                if (pairIter.first != allProj.end()){
                    for (auto chosenProj=pairIter.first; chosenProj != pairIter.second; chosenProj++) {
                        //chosenProj is a project begins with synState.
                        projectSet set;
                        set.insert(*chosenProj);
                        bool subset=in(set ,clos);
                        bool connected = false;
                        if (dotpos+1 < right.size()){
                            auto first=firstSets[gotoColNum(right[dotpos+1])];
                            // chosen look in first.
                            connected=(find(first.begin(), first.end(), chosenProj->second.look)!=first.end());
                        }
                        else {
                            connected = (chosenProj->second.look==TYPE::HEAD);
                        }
                        if (chosenProj->second.dotpos==0 && !subset && connected){
                            newClos.insert(*chosenProj);
                        }
                    }
                } 
            }
        }
    }while(newClos.size()!=clos.size());
    return clos;
}

static inline projectSet gotoFuc(projectSet cur, rightElem input)
{
    projectSet dest;
    for(auto iter=cur.begin(); iter != cur.end(); iter++) {
        int dotpos = iter->second.dotpos;
        vector<rightElem> right = iter->second.right;
        if (dotpos < right.size() && right[dotpos].type==input.type && right[dotpos].index == input.index) {
            projectRight pRight{right, dotpos+1, iter->second.look};
            dest.insert({iter->first, pRight});
        }
    }
    return closure(dest);
}

static inline int locateDFAstate(vector<DFAstate> DFAstates, projectSet I)
{
    auto stateIter=DFAstates.begin();
    bool found = false;
    for (; stateIter!=DFAstates.end(); stateIter++) {
        // check if they are exactly the same.
        auto iter1=stateIter->pSet.begin();
        auto iter2=I.begin();
        found = true;
        for (; iter1 !=stateIter->pSet.end() && iter2 !=I.end(); iter1++, iter2++) {
            if (iter1->first!=iter2->first ||
                iter1->second.dotpos!=iter2->second.dotpos || iter1->second.look != iter2->second.look) {
                found = false;
                break;
            }
            auto subIter1 = iter1->second.right.begin();
            auto subIter2 = iter2->second.right.begin();
            for (; subIter1!=iter1->second.right.end() && subIter2!=iter2->second.right.end(); subIter1++, subIter2++) {
                if (subIter1->index!=subIter2->index || subIter1->type!=subIter2->type) {
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

static inline int locateFormula(pair<SYNTAX_STATE, vector<rightElem>> formula)
{
    int cnt = 0;
    for (auto iter=grammar.begin(); iter != grammar.end(); iter++, cnt++) {
        // check if *iter and formula are exactly the same.
        if (iter->first!=formula.first || iter->second.size()!=formula.second.size()) {
            continue;
        }
        auto gIter = iter->second.begin();
        auto fIter = formula.second.begin();
        bool found = true;
        for (; gIter != iter->second.end() && fIter != formula.second.end(); gIter++, fIter++) {
            if (gIter->index!=fIter->index || gIter->type!=fIter->type) {
                found = false;
                break;
            }
        }
        if (found)
            break;
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

static inline vector<DFAstate> generateDFA(vector<vector<int>> &gotoTable)
{
    vector<DFAstate> DFA, newDFA;
    rightElem initRight={RIGHT_ELEM_TYPE::STATE,SYNTAX_STATE::BEG};
    auto follow=followSets[gotoColNum(initRight)];
    projectSet init;
    for (auto iter=follow.begin(); iter!=follow.end(); iter++)
        init.insert({SYNTAX_STATE::SBEG,{{initRight}, 0, *iter}});
    int num = 0;
    newDFA.push_back({num, closure(init)});
    do {
        DFA=newDFA;
        for (auto iter=DFA.begin(); iter!=DFA.end(); iter++) {
            for (SYNTAX_STATE synState = SYNTAX_STATE::SBEG; synState <= SYNTAX_STATE::V; synState=(SYNTAX_STATE)(synState+1)) {
                auto nextSet = gotoFuc(iter->pSet, {RIGHT_ELEM_TYPE::STATE,synState});
                if (nextSet.size()) {
                    int dest = locateDFAstate(DFA,nextSet);
                    if (dest == DFA.size()) {
                        vector<int> gotoRow(60);
                        gotoTable.push_back(gotoRow);
                        newDFA.push_back({++num, nextSet});
                    }
                    gotoTable[iter->num][gotoColNum(RIGHT_ELEM_TYPE::STATE, synState)]=dest;
                }
            }
            for (enum TYPE type=TYPE::HEAD; type < TYPE::TAIL; type = (enum TYPE)(type+1)) {
                auto nextSet = gotoFuc(iter->pSet, {RIGHT_ELEM_TYPE::TERMINATER,type});
                if (nextSet.size()) {
                    int dest = locateDFAstate(DFA,nextSet);
                    if (dest==DFA.size()) {
                        vector<int> gotoRow(60);
                        gotoTable.push_back(gotoRow);
                        newDFA.push_back({++num, nextSet});
                    }
                    gotoTable[iter->num][gotoColNum(RIGHT_ELEM_TYPE::TERMINATER, type)]=dest;
                }
            }
        }
    }while(newDFA.size()!=DFA.size());
    return DFA;
}

static inline void initActionTable(const vector<DFAstate> DFA, vector<vector<actionElem>> &actionTable)
{
    for (int i =0 ; i<DFA.size(); i++){
        vector<actionElem> actionRow;
        for (enum TYPE type=TYPE::HEAD; type < TYPE::TAIL; type = (enum TYPE)(type+1)){
            actionRow.push_back({GOTO_ELEM_TYPE::ERR, -1});
        }
        actionTable.push_back(actionRow);
    } 
}

static inline void generateAction(vector<DFAstate> DFA, vector<vector<int>> gotoTable, vector<vector<actionElem>> &actionTable)
{
    long long x = 0;
    for (auto DFAstate=DFA.begin(); DFAstate!=DFA.end(); DFAstate++) {
        for (auto iter=DFAstate->pSet.begin(); iter !=DFAstate->pSet.end(); iter++) {
            x++;
            int dotpos = iter->second.dotpos;
            vector<rightElem> right = iter->second.right;
            int curStateNum = DFAstate-DFA.begin();
            
            if(dotpos==right.size()) {
                int nextFormulaNum = locateFormula({iter->first, iter->second.right});
                actionTable[curStateNum][iter->second.look].gotoElemType = GOTO_ELEM_TYPE::INDUCE;
                actionTable[curStateNum][iter->second.look].des=nextFormulaNum;
            }
            else if (right[dotpos].type==RIGHT_ELEM_TYPE::TERMINATER) {
                actionTable[curStateNum][right[dotpos].index].gotoElemType=GOTO_ELEM_TYPE::MOVE;
                actionTable[curStateNum][right[dotpos].index].des=gotoTable[curStateNum][gotoColNum(RIGHT_ELEM_TYPE::TERMINATER, right[dotpos].index)];
            }
        }
    }
}

static inline void saveTable(const char* actionPath, const char* gotoPath, const vector<vector<int>> gotoTable, const vector<vector<actionElem>> actionTable)
{
    string outputPath = actionPath;
    ofstream outfile;
    outfile.open(outputPath);
    if (outfile.fail()) {
        cout << "Open " << outputPath << " failed" << endl;
    }
    outfile << setiosflags(ios::left);
    for (enum TYPE type=TYPE::HEAD; type < TYPE::TAIL; type = (enum TYPE)(type+1)) {
        outfile << setw(20) << names[type];
    }
    outfile << endl;
    for (int i =0;i < actionTable.size(); i++) {
        for (int j =0;j < actionTable[i].size(); j++) {
            if (actionTable[i][j].gotoElemType==GOTO_ELEM_TYPE::MOVE){
                outfile << "S" << actionTable[i][j].des << setw(18) << " ";
            }
            else if (actionTable[i][j].gotoElemType==GOTO_ELEM_TYPE::INDUCE){
                outfile << "R" << actionTable[i][j].des << setw(18) << " ";
            } 
            else
                outfile << "E"<< setw(29) << " ";
        }
        outfile << endl;
    }
    outfile.close();

    outputPath = gotoPath;
    outfile.open(outputPath);
    if (outfile.fail()) {
        cout << "Open " << outputPath << " failed" << endl;
    }
    for (SYNTAX_STATE synState = SYNTAX_STATE::SBEG; synState <= SYNTAX_STATE::V; synState=(SYNTAX_STATE)(synState+1)) {
        outfile << setw(4) << stateNames[synState];
    }
    outfile << endl;
    for (int i =0;i < gotoTable.size(); i++) {
        for (int j =0;j < gotoTable[i].size(); j++) {
            outfile << setw(4) << gotoTable[i][j];
        }
        outfile << endl;
    }
    outfile.close();
}

void generateLRTable(vector<vector<int>> &gotoTable, vector<vector<actionElem>> &actionTable)
{
    generateFirst();
    generateFollow();
    getAllProjects();
    auto DFA=generateDFA(gotoTable);
    initActionTable(DFA, actionTable);
    generateAction(DFA, gotoTable, actionTable);
    saveTable("/mnt/Data/Programming/my-wife/Analyzer/data/Actiontable.txt", "/mnt/Data/Programming/my-wife/Analyzer/data/GOTOtable.txt", gotoTable, actionTable);
}

// int main()
// {
//     generateLRTable();
//     return 0;
// }