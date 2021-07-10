#include <iostream>
#include <algorithm>

#include "calculator.h"
#include "display.h"
#include "terminal.h"

using std::cin;
using std::cout;
using std::endl;

using std::find;

const string welcomeMsg = "Calculator by 0nchy, enjoy yourself ~";
const string versionMsg = "Version 3.2.9 on linux";
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
			cout << " x = " << ep.x << endl << "fx = " << ep.fx << endl;
			continue;
		}
		else if (s.at(0) == asgnCmd) {
			s = s.erase(0, 1);
			if (s.empty()) {
				ep.x = 0;
				continue;
			}
			outPos = 1;
			asgn = true;
		}
		else if (s.at(0) == funcCmd) {
			function = s.erase(0, 1);
			if (s.empty()) {
				ep.fx = "";
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
			if (!ep.accurate) {
				DisplayResult(ep.getResult(), base_str);
				// cout << ep.getResult() << endl;
			}
			else {
				printf("%.10lf\n", ep.result);
			}
		}
		else {
			DisplayErrorMsg(ep, outPos);
		}
	}
	return 0;
}
