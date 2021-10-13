/*
 * Created Date: 2021-10-12 21:30:40
 * Author: yuyangz
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Lexical.h"
#include "LexicalProcess.cpp"

using namespace std;

int main()
{
    ifstream infile;
    infile.open("../data/input.cpp");
    if (infile.fail()) {
        cout << "Open input file failed" << endl;
    }

    string file;
    char data[10000];
    infile.read(data, 10000);
    file = data;
    infile.close();

    vector<token> anaRes = mainProcess(file);

    ofstream outfile;
    outfile.open("../data/output.txt");
    if (infile.fail()) {
        cout << "Open output file failed" << endl;
    }

    for (vector<token>::iterator it = anaRes.begin(); it != anaRes.end(); it++) {
        outfile << "<" << (*it).type  << ", "<< (*it).value << ">" << endl;
    }
    outfile.close();

    return 0;
}