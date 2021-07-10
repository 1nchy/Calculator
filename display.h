#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <iostream>
#include <string>
#include <cstring>
#include <set>

#include "calculator.h"

using std::string;
using std::set;
using std::cout;
using std::endl;

const string inputNotice = ">>>";
const set<string> echoSet {
    string("echo")
};
const set<string> helpSet {
    string("help")
};
const set<string> quitSet {
    string("quit"), string("q"), string("Q")
};

void DisplayErrorMsg(Expression&, unsigned outPos);
void DisplayOperators();
void DisplayResult(const double &, const string &base_str);

#endif
