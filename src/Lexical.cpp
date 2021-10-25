/*
 * Created Date: 2021-10-12 21:30:40
 * Author: yuyangz
 */

#include "Lexical.h"

using namespace std;


int main()
{
    fileProcess("../data/input_correct.cpp", "../data/output_correct.txt");
    fileProcess("../data/input_wrong.cpp", "../data/output_wrong.txt");
    return 0;
}