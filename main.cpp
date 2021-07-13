#include <iostream>
#include <algorithm>

#include "calculator.h"
#include "display.h"
#include "terminal.h"

using std::cin;
using std::cout;
using std::endl;

using std::find;

const string welcomeMsg = "Calculator by 1nchy, enjoy yourself ~";
const string versionMsg = "Version 3.3.1 on linux";
// const string inputNotice = ">>>";

int main(int argc, char *argv[]) {
	cout << welcomeMsg << endl;
	cout << versionMsg << endl;
	Expression ep;
	string function;
    terminal t(inputNotice);
	while (1) {
		bool asgn = false, asfx = false;
		unsigned outPos = 0;

		string s = t.wait();
        if (t.eof()) {
            break;
        }
		// s 一定非空且非全空格
		if (find(quitSet.cbegin(), quitSet.cend(), s) != quitSet.cend()) {
			break;
		}
		else if (find(helpSet.cbegin(), helpSet.cend(), s) != helpSet.cend()) {
			DisplayOperators();
			continue;
		}
		else if (find(echoSet.cbegin(), echoSet.cend(), s) != echoSet.cend()) {
			cout << " x = " << ep.getx() << endl << "fx = " << ep.getfx() << endl;
			continue;
		}
		else if (s.at(0) == asgnCmd) {
			s = s.erase(0, 1);
			if (s.empty()) {
				ep.xzero();
				continue;
			}
			outPos = 1;
			asgn = true;
		}
		else if (s.at(0) == funcCmd) {
			function = s.erase(0, 1);
			if (s.empty()) {
				ep.fxzero();
				continue;
			}
			outPos = 1;
			asfx = true;
			// ep.func = function;
		}

		string base_str;
		auto bpos = s.rfind(baseTrans);
		if (bpos != string::npos) {
			base_str = s.substr(bpos+2);
			// cout << base_str << endl;
			s.erase(bpos);
		}
		ep.init(s, asgn, asfx);
		if (ep.compute() == CORRECT) {
			if (!ep.accurate()) {
				cout << ep.to_string(base_str) << endl;
			}
			else {
				cout << ep.to_string(1) << endl;
			}
		}
		else {
			DisplayErrorMsg(ep.getErrorInfo(), outPos);
		}
	}
	return 0;
}
