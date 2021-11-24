/*
 * Created Date: 2021-11-24 15:54:20
 * Author: yuyangz
 */



#include "../Parser/Parser.h"

int main()
{
    string inputPath = "data/input_normal.cpp";
    ifstream infile;
    infile.open(inputPath);
    if (infile.fail()) {
        cout << "Open " << inputPath << " failed" << endl;
    }

    string file;
    char data[10000];
    memset(data, 0, 10000*sizeof(char));
    infile.clear();
    infile.read(data, 10000);
    file = data;
    infile.close();

    vector<token> anaRes = lexicalProcess(file);
    vector<analyzeProcess> processTable;
    analyzeTree t;
    string errlog;

    parserProcess(anaRes, processTable, t, errlog);


    string outputPath = "data/table.txt";
    ofstream outfile;
    outfile.open(outputPath);
    if (outfile.fail()) {
        cout << "Open " << outputPath << " failed" << endl;
    }

    outfile << setw(25) << "state stack" << setw(25) << "symbol stack"<<  setw(25) << "input string" << endl;
    for (auto iter=processTable.begin(); iter < processTable.end(); iter++) {
        while(iter->stateStack.size()) {
            outfile << iter->stateStack.top();
            iter->stateStack.pop();
        }
        outfile << "\t";
        while(iter->symbolStack.size()) {
            auto symbol = iter->symbolStack.top();
            if(symbol.type==RIGHT_ELEM_TYPE::STATE) {
                outfile << stateNames[symbol.index];
            }
            else {
                outfile << names[symbol.index];
            }
            iter->symbolStack.pop();
        }
        outfile << "\t";
        outfile << iter->inputString[0].value;
        outfile << endl;
    }
    outfile.close();
}