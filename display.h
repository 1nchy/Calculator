#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <iostream>
#include <string>
#include <set>

#include "calculator.h"

using std::string;
using std::set;
using std::cout;
using std::endl;

const string welcomeMsg = "Calculator by 1nchy, enjoy yourself ~";
const string versionMsg = "Version 3.2.3 on linux";
const string inputNotice = ">>> ";
const char asgnCmd = ':';
const char funcCmd = '\\';
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

#endif
