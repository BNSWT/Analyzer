/*
 * Created Date: 2021-10-12 21:30:40
 * Author: yuyangz
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Lexical.h"
#include "LexicalDFA.cpp"
#include "LexicalProcess.cpp"

using namespace std;

int main()
{
    ifstream infile;
    infile.open("./data/input.txt");

    string file;
    infile >> file;
    infile.close();

    vector<string> anaRes = mainProcess(file);

    ofstream outfile;
    outfile.open("./data/output.txt");

    for (vector<string>::iterator it = anaRes.begin(); it != anaRes.end(); it++) {
        outfile << *it << endl;
    }

    outfile.close();

    return 0;
}