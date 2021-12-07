/*
 * Created Date: 2021-11-17 09:20:13
 * Author: yuyangz
 */


#include"Parser.h"

static vector<analyzeProcess> parseProcess;
static string errlog;

extern int gotoColNum(RIGHT_ELEM_TYPE rType, int index);
extern pair<SYNTAX_STATE, vector<rightElem>> findFormula(int cnt);

static inline void readTable(const char* actionPath, const char* gotoPath, vector<vector<int>> &gotoTable,vector<vector<actionElem>> &actionTable)
{
    string inputPath = actionPath;
    ifstream infile;
    infile.open(inputPath);
    if (infile.fail()) {
        cout << "Open " << inputPath << " failed" << endl;
    }
    string names;
    getline(infile, names);
    while(!infile.fail()){
        string line;
        getline(infile, line);
        if (line.empty()) {
            continue;
        }
        vector<actionElem> actionElems;
        stringstream ss(line);
        while(!ss.fail()){
            actionElem elem;
            char ch;
            ss>>ch;
            if (ss.fail())
                break;
            if (ch=='S') {
                elem.gotoElemType = GOTO_ELEM_TYPE::MOVE;
                ss>>elem.des;
                actionElems.push_back(elem);
            }
            else if (ch=='R') {
                elem.gotoElemType = GOTO_ELEM_TYPE::INDUCE;
                ss>>elem.des;
                actionElems.push_back(elem);
            }
            else if (ch=='E'){
                elem.gotoElemType = GOTO_ELEM_TYPE::ERR;
                elem.des=-1;
                actionElems.push_back(elem);
            }
        }
        actionTable.push_back(actionElems);
    }

    infile.close();

    inputPath = gotoPath;
    infile.open(inputPath);
    if (infile.fail()) {
        cout << "Open " << inputPath << " failed" << endl;
    }
    getline(infile, names);
    while(!infile.fail()){
        string line;
        getline(infile, line);
        if (line.empty()) {
            continue;
        }
        vector<int> gotoElems;
        stringstream ss(line);
        while(!ss.fail()){
            int elem;
            ss>>elem;
            if (!ss.fail())
                gotoElems.push_back(elem);
        }
        gotoTable.push_back(gotoElems);
    }
    infile.close();
}

static inline vector<token> readToken(const char* tokenPath)
{
    vector<token> res;

    ifstream infile;
    infile.open(tokenPath);
    if (infile.fail()) {
        cout << "Open " << tokenPath << " failed" << endl;
    }

    while(!infile.fail()) {
        int type;
        infile >> type;
        string value;
        infile >> value;
        if (infile.fail())
            break;
        res.push_back({(enum TYPE)type, value});
    }
    infile.close();
    return res;
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

static inline void mainProcess(vector<token> &inputString, const vector<vector<int>> gotoTable, const vector<vector<actionElem>> actionTable)
{
    int curState = 0;
    multimap<rightElem, vector<rightElem>> treeInfo;
    while(inputString.size()) {
        auto term=inputString.front();
        int termID = term.type;
        actionElem aElem = actionTable[curState][termID];
        stack<int> stateStack;
        stack<rightElem> elemStack; 
        if (parseProcess.size()) {
            stateStack = parseProcess[parseProcess.size()-1].stateStack;
            elemStack = parseProcess[parseProcess.size()-1].symbolStack; 
        }
        switch(aElem.gotoElemType) {
            case GOTO_ELEM_TYPE::MOVE:
                stateStack.push(aElem.des);
                curState = aElem.des;
                elemStack.push({RIGHT_ELEM_TYPE::TERMINATER,termID});
                parseProcess.push_back({stateStack, elemStack, inputString});
                inputString.erase(inputString.begin());
                break;
            case GOTO_ELEM_TYPE::INDUCE:{
                auto formula = findFormula(aElem.des);
                vector<rightElem> children;
                for (int i =0; i < formula.second.size(); i++) {
                    children.push_back(elemStack.top());
                    // if (formula.second[i].type==elemStack.top().type && formula.second[i].index==elemStack.top().index) {
                    stateStack.pop();
                    elemStack.pop();
                    // }
                }
                auto left = formula.first;
                if (stateStack.size())
                    curState = stateStack.top();
                else
                    curState = 0;
                auto nextState = gotoTable[curState][gotoColNum(RIGHT_ELEM_TYPE::STATE,formula.first)];
                rightElem nextRightElem{RIGHT_ELEM_TYPE::STATE, left};
                stateStack.push(nextState);
                curState = nextState;
                elemStack.push(nextRightElem);
                parseProcess.push_back({stateStack, elemStack, inputString});
                //treeInfo.insert({nextRightElem, children});
                break;
            }
            default:
                errlog = wrongHint(term);
                break;
        }
        if (errlog.size())
            break;

    }
}

static inline void saveProcess()
{
    string outputPath = "/home/yuyangz/Documents/courses/compilation/Analyzer/data/table.txt";
    ofstream outfile;
    outfile.open(outputPath);
    if (outfile.fail()) {
        cout << "Open " << outputPath << " failed" << endl;
    }

    outfile << setw(25) << "state stack" << setw(25) << "symbol stack"<<  setw(25) << "input string" << endl;
    for (auto iter=parseProcess.begin(); iter < parseProcess.end(); iter++) {
        while(iter->stateStack.size()) {
            outfile << iter->stateStack.top();
            iter->stateStack.pop();
        }
        outfile << "\t";
        while(iter->symbolStack.size()) {
            auto symbol = iter->symbolStack.top();
            if(symbol.type==RIGHT_ELEM_TYPE::STATE) {
                outfile << stateNames[symbol.index] << " ";
            }
            else {
                outfile << names[symbol.index] << " ";
            }
            iter->symbolStack.pop();
        }
        outfile << "\t";
        if (iter->inputString.size())
            outfile << iter->inputString[0].value;
        outfile << endl;
    }
    outfile.close();
}

string parserProcess(vector<token> &inputString, const vector<vector<int>> gotoTable, const vector<vector<actionElem>> actionTable)
{
    // readTable("/mnt/Data/Programming/my-wife/Analyzer/data/Actiontable.txt", "/mnt/Data/Programming/my-wife/Analyzer/data/GOTOtable.txt");
    mainProcess(inputString, gotoTable, actionTable);
    saveProcess();
    return errlog;
}

int main()
{
    vector<token> inputString=readToken("/home/yuyangz/Documents/courses/compilation/Analyzer/data/output_normal.txt");
    inputString.push_back({TYPE::TAIL, "end of file"});
    vector<vector<int>> gotoTable;
    vector<vector<actionElem>> actionTable;
    // generateLRTable(gotoTable, actionTable);

    // vector<vector<int>> read_gotoTable;
    // vector<vector<actionElem>> read_actionTable;
    readTable("/home/yuyangz/Documents/courses/compilation/Analyzer/data/Actiontable.txt", "/home/yuyangz/Documents/courses/compilation/Analyzer/data/GOTOtable.txt", gotoTable, actionTable);

    // //compare action
    // if (actionTable.size()!=read_actionTable.size()) {
    //     cout << "action row num wrong!" << endl;
    // }
    // else {
    //     for (int i =0; i<actionTable.size(); i++) {
    //         if (actionTable[i].size()!=read_actionTable[i].size()) {
    //             cout << "action row " << i << " col num wrong " << endl;
    //             continue;
    //         }
    //         for (int j = 0; j < actionTable[i].size();j++) {
    //             if (actionTable[i][j].des!=read_actionTable[i][j].des) {
    //                 cout << "action[" << i << "][" << j << "] des wrong" << endl;
    //             }
    //             if (actionTable[i][j].gotoElemType!=read_actionTable[i][j].gotoElemType) {
    //                 cout << "action[" << i << "][" << j << "] type wrong" << endl;
    //             }
    //         }
    //     }
    // }
    // cout << "end of action test" << endl;

    // //compare goto
    // if (gotoTable.size()!=read_gotoTable.size()) {
    //     cout << "goto row num wrong!" << endl;
    // }
    // else {
    //     for (int i =0; i<gotoTable.size(); i++) {
    //         if (gotoTable[i].size()!=read_gotoTable[i].size()) {
    //             cout << "goto row " << i << " col num wrong " << endl;
    //             continue;
    //         }
    //         for (int j = 0; j < gotoTable[i].size();j++) {
    //             if (gotoTable[i][j]!=read_gotoTable[i][j]) {
    //                 cout << "goto[" << i << "][" << j << "] wrong" << endl;
    //             }
    //         }
    //     }
    // }
    // cout << "end of goto test" << endl;

    parserProcess(inputString, gotoTable, actionTable);

    return 0;
}