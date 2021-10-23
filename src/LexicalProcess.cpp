/*
 * Created Date: 2021-10-12 21:04:04
 * Author: yuyangz
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "Lexical.h"
#include "LexicalDFA.cpp"

using namespace std;

void deFormat(string &source)
{
    
    // deComment
    for (int i = 0; i < source.size(); i++) {
        if (source[i] == '/' && i+1 < source.size()) {
            int pos = i;
            int j = i+1;
            if (source[j] == '/') {
                int npos = 2;
                while(j < source.size() && source[j]!='\n' && source[j]!='\r') {
                    j++;
                    npos++;
                }
                source.erase(pos, npos);
            }
            else if (source[j] == '*') {
                int npos = 2;
                while(j < source.size() && (source[j]!='/' || source[j-1]!='*')) {
                    j++;
                    npos++;
                }
                source.erase(pos, npos);
            }
        }
    }
    
    //deSpace, deEndl
    stringstream is(source);
    string res;
    string tmp;
    while(is >> tmp) {
        res += tmp;
        res += ' ';
    }

    source = res;

    return;
}

vector<token> mainProcess(string source)
{
    deFormat(source);
    //cout << source << endl;
    STATE state = STATE::INIT;

    vector<token> res;
    string buffer;
    for (int i = 0; i < source.size(); i++){
        state = findNext(state, source[i], buffer, res);
    }
    return res;
}

void fileProcess(const char* inputPath, const char* outputPath)
{
    ifstream infile;
    infile.open(inputPath);
    if (infile.fail()) {
        cout << "Open " << inputPath << " failed" << endl;
    }

    string file;
    char data[10000];
    infile.read(data, 10000);
    file = data;
    infile.close();

    vector<token> anaRes = mainProcess(file);

    ofstream outfile;
    outfile.open(outputPath);
    if (outfile.fail()) {
         cout << "Open " << outputPath << " failed" << endl;
    }

    for (vector<token>::iterator it = anaRes.begin(); it != anaRes.end(); it++) {
        outfile << "<" << (*it).type  << ", "<< (*it).value << ">" << endl;
    }
    outfile.close();
}