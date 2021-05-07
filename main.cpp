#include <iostream>
#include <algorithm>

#include "calculator.h"
#include "display.h"

using std::cin;
using std::cout;
using std::endl;

using std::find;

int main(int argc, char *argv[]) {
	cout << welcomeMsg << endl;
	cout << versionMsg << endl;
	Expression ep;
	string function;
	while (1) {
		string s;
		bool asgn = false, asfx = false;
		unsigned outPos = 0;
		do {
			printf(inputNotice.c_str());
			if (std::getline(cin, s).eof()) {
				return 0;
			}
		} while (s.empty() || s.compare(string(s.size(), ' ')) == 0);
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
		ep.init(s, asgn, asfx);
		if (ep.compute() == CORRECT) {
			if (!ep.accurate) {
				cout << ep.getResult() << endl;
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
