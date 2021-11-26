/*
 * Created Date: 2021-11-17 09:20:13
 * Author: yuyangz
 */


#include"Parser.h"

static vector<analyzeProcess> parseProcess;
static string errlog;
static vector<vector<int>> gotoTable;
static vector<vector<actionElem>> actionTable;

extern pair<SYNTAX_STATE, vector<rightElem>> findFormula(int cnt);

extern inline int gotoColNum(RIGHT_ELEM_TYPE rType, int index);

static inline void readTable(const char* actionPath, const char* gotoPath)
{
    string inputPath = actionPath;
    ifstream infile;
    infile.open(inputPath);
    if (infile.fail()) {
        cout << "Open " << inputPath << " failed" << endl;
    }
    string names;
    getline(infile, names);
    for (int i =0;i < actionTable.size(); i++) {
        for (int j =0;j < actionTable[i].size(); j++) {
            char gotoType;
            infile >> gotoType;
            if (gotoType == 'S')
                actionTable[i][j].gotoElemType=GOTO_ELEM_TYPE::MOVE;
            else if (gotoType == 'R')
                actionTable[i][j].gotoElemType=GOTO_ELEM_TYPE::INDUCE;
            else
                actionTable[i][j].gotoElemType=GOTO_ELEM_TYPE::ERR;
            infile >> actionTable[i][j].des;
        }
    }
    infile.close();

    inputPath = gotoPath;
    infile.open(inputPath);
    if (infile.fail()) {
        cout << "Open " << inputPath << " failed" << endl;
    }
    getline(infile, names);
    for (int i =0;i < gotoTable.size(); i++) {
        for (int j =0;j < gotoTable[i].size(); j++) {
            infile >> gotoTable[i][j];
        }
    }
    infile.close();
}

static inline analyzeTree makeTree(multimap<rightElem, vector<rightElem>> treeInfo)
{
    analyzeTree t = NULL;
    return t;
}

static inline string wrongHint(token term)
{
    return  "the" + names[term.type] + " " + term.value + "is not recognized";
}

static inline void mainProcess(vector<token> &inputString)
{
    int curState = 0;
    multimap<rightElem, vector<rightElem>> treeInfo;
    while(inputString.size()) {
        auto term=inputString.front();
        int termID = term.type;
        inputString.erase(inputString.begin());
        // actionElem aElem = actionTable[curState][termID];
        // stack<int> stateStack;
        // stack<rightElem> elemStack; 
        // if (parseProcess.size()) {
        //     stateStack = parseProcess[parseProcess.size()-1].stateStack;
        //     elemStack = parseProcess[parseProcess.size()-1].symbolStack; 
        // }
        // switch(aElem.gotoElemType) {
        //     case GOTO_ELEM_TYPE::MOVE:
        //         stateStack.push(aElem.des);
        //         elemStack.push({RIGHT_ELEM_TYPE::TERMINATER,termID});
        //         parseProcess.push_back({stateStack, elemStack, inputString});
        //         break;
        //     case GOTO_ELEM_TYPE::INDUCE:{
        //         auto formula = findFormula(aElem.des);
        //         vector<rightElem> children;
        //         for (int i =0; i < formula.second.size(); i++) {
        //             children.push_back(elemStack.top());
        //             stateStack.pop();
        //             elemStack.pop();
        //         }
        //         auto left = formula.first;
        //         auto nextState = gotoTable[curState][formula.first];
        //         rightElem nextRightElem{RIGHT_ELEM_TYPE::STATE, left};
        //         stateStack.push(nextState);
        //         elemStack.push(nextRightElem);
        //         parseProcess.push_back({stateStack, elemStack, inputString});
        //         //treeInfo.insert({nextRightElem, children});
        //         break;
        //     }
        //     default:
        //         errlog = wrongHint(term);
        //         break;
        // }
        // if (errlog.size())
        //     break;
    }
}

static inline void saveProcess()
{
    
}

void parserProcess(vector<token> &inputString)
{
    readTable("data/Actiontable.txt", "data/GOTOtable.txt");
    mainProcess(inputString);
}