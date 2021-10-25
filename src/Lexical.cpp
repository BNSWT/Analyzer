/*
 * Created Date: 2021-10-12 21:30:40
 * Author: yuyangz
 */

#include "Lexical.h"

using namespace std;


int main()
{
    fileProcess("../data/input_normal.cpp", "../data/output_normal.txt");
    fileProcess("../data/input_undefined.cpp", "../data/output_undefined.txt");
    fileProcess("../data/input_bad_float.cpp", "../data/output_bad_float.txt");
    fileProcess("../data/input_bad_string.cpp", "../data/output_bad_string.txt");
    return 0;
}