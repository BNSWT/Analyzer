/*
 * Created Date: 2021-11-17 09:22:27
 * Author: yuyangz
 */

#include "Parser.h"

static projectSet allProj;
static vector<vector<int>> gotoTable;
static vector<vector<actionElem>> actionTable;

static inline void getAllProjects()
{
    for (SYNTAX_STATE synState = SYNTAX_STATE::SBEG; synState <= SYNTAX_STATE::V; synState=(SYNTAX_STATE)(synState+1)) {
        auto pairIter = grammar.equal_range(synState);
        if (pairIter.first != grammar.end()) {
            for (auto iter = pairIter.first; iter != pairIter.second; iter++) {
                vector<rightElem> formula= iter->second;
                for (int i=0; i<=formula.size(); i++) {
                    projectRight pRight{formula, i};
                    allProj.insert({synState, pRight});
                }
            }
        }
    }
}

static inline projectSet formula2projects(pair<SYNTAX_STATE, vector<rightElem>> formula)
{
    projectSet set;
    for (int i = 0; i < formula.second.size(); i++) {
        projectRight pRight{formula.second, i};
        set.insert({formula.first, pRight});
    }
    return set;
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
                auto pairIter = allProj.equal_range(synState);
                if (pairIter.first != allProj.end()){
                    for (auto chosenProj=pairIter.first; chosenProj != pairIter.second; chosenProj++) {
                        projectSet set;
                        set.insert(*chosenProj);
                        bool subset=in(set ,clos);
                        if (chosenProj->second.dotpos==0 && !subset){
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
            projectRight pRight{right, dotpos+1};
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
        auto iter1=stateIter->pSet.begin();
        auto iter2=I.begin();
        found = true;
        for (; iter1 !=stateIter->pSet.end() && iter2 !=I.end(); iter1++, iter2++) {
            if (iter1->first!=iter2->first ||
                iter1->second.dotpos!=iter2->second.dotpos) {
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
    for (auto iter=grammar.begin(); iter != grammar.end(); iter++) {
        cnt++;
        if (iter->first!=formula.first) {
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

static inline pair<SYNTAX_STATE, vector<rightElem>> findFormula(int cnt)
{
    auto iter=grammar.begin();
    for (int i = 0; i < cnt && iter != grammar.end(); iter++, i++)
    ;
    return *iter;
}

int gotoColNum(RIGHT_ELEM_TYPE rType, int index)
{
    if (rType ==RIGHT_ELEM_TYPE::STATE)
        return index;
    else
        return index+20;
}

static inline vector<DFAstate> generateDFA()
{
    vector<DFAstate> DFA, newDFA;
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
    int num = 0;
    newDFA.push_back({num, closure(init)});
    do {
        DFA=newDFA;
        for (auto iter=DFA.begin(); iter!=DFA.end(); iter++) {
            for (SYNTAX_STATE synState = SYNTAX_STATE::SBEG; synState <= SYNTAX_STATE::V; synState=(SYNTAX_STATE)(synState+1)) {
                auto nextSet = gotoFuc(iter->pSet, {RIGHT_ELEM_TYPE::STATE,synState});
                if (nextSet.size()&&locateDFAstate(DFA,nextSet)==DFA.size()) {
                    vector<int> gotoRow(60);
                    gotoTable.push_back(gotoRow);
                    newDFA.push_back({++num, nextSet});
                    gotoTable[iter->num][gotoColNum(RIGHT_ELEM_TYPE::STATE, synState)]=num;
                }
            }
            for (enum TYPE type=TYPE::HEAD; type < TYPE::TAIL; type = (enum TYPE)(type+1)) {
                auto nextSet = gotoFuc(iter->pSet, {RIGHT_ELEM_TYPE::TERMINATER,type});
                if (nextSet.size()&&locateDFAstate(DFA,nextSet)==DFA.size()) {
                    vector<int> gotoRow(60);
                    gotoTable.push_back(gotoRow);
                    newDFA.push_back({++num, nextSet});
                    gotoTable[iter->num][gotoColNum(RIGHT_ELEM_TYPE::TERMINATER, type)]=num;
                }
            }
        }
    }while(newDFA.size()!=DFA.size());
    return DFA;
}

static inline void initActionTable(const vector<DFAstate> DFA)
{
    for (int i =0 ; i<DFA.size(); i++){
        vector<actionElem> actionRow;
        for (SYNTAX_STATE synState = SYNTAX_STATE::SBEG; synState <= SYNTAX_STATE::V; synState=(SYNTAX_STATE)(synState+1)) {
            actionRow.push_back({GOTO_ELEM_TYPE::ERR, -1});
        }
        actionTable.push_back(actionRow);
    } 
}

static inline void generateAction(const vector<DFAstate> &DFA)
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
                for (int i =0;i<actionTable[0].size();i++){
                    actionTable[curStateNum][i].gotoElemType = GOTO_ELEM_TYPE::INDUCE;
                    actionTable[curStateNum][i].des=nextFormulaNum;
                } 
            }
            else if (right[dotpos].type==RIGHT_ELEM_TYPE::TERMINATER) {
                actionTable[curStateNum][right[dotpos].index].gotoElemType=GOTO_ELEM_TYPE::MOVE;
                actionTable[curStateNum][right[dotpos].index].des=gotoTable[curStateNum][gotoColNum(RIGHT_ELEM_TYPE::TERMINATER, right[dotpos].index)];
            }
        }
    }
}

static inline void saveTable(const char* actionPath, const char* gotoPath)
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
                outfile << setw(20) << " ";
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

void generateLRTable()
{
    getAllProjects();
    auto DFA=generateDFA();
    initActionTable(DFA);
    generateAction(DFA);
    saveTable("Actiontable.txt", "GOTOtable.txt");
}

int main()
{
    generateLRTable();

    return 0;
}