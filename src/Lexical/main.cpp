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

    generateLRTable();
    // parserProcess(anaRes);

    return 0;
}