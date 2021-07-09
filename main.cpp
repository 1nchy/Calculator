#include <iostream>
#include <algorithm>

#include "calculator.h"
#include "display.h"

using std::cin;
using std::cout;
using std::endl;

using std::find;

const string welcomeMsg = "Calculator by 0nchy, enjoy yourself ~";
const string versionMsg = "Version 3.2.7 on linux";

int main(int argc, char *argv[]) {
	cout << welcomeMsg << endl;
	cout << versionMsg << endl;
	Expression ep;
	string function;
	vector<string> history;
	vector<string>::const_iterator cur;
	while (1) {
		printf("%s", inputNotice.c_str());
		bool asgn = false, asfx = false;
		unsigned outPos = 0;
		string s;
		int c;
		string::const_iterator cursor = s.cbegin();
		cur = history.cend();
		system("stty raw");
		while ((c = getchar()) != EOF) {
			if (c == '\r' || c == '\n') {
				printf("\b\b");
				printf("  ");
				printf("\b\b");
				if (s.empty() || s.compare(string(s.size(), ' ')) == 0) {
					continue;
				}
				for (auto iter = cursor; iter != s.cend(); ++iter) {
					printf("%c", *iter);
				}
				system("/bin/stty cooked");
				cout << endl;
				history.push_back(s);
				break;
			}
			// ctrl+c or ctrl+d
			else if (c == '\04' || c == '\03') {
                system("/bin/stty cooked");
				cout << endl;
				return 0;
			}
			else if (c == '\033') {
				getchar();
				int arrow = getchar();
				printf("\b\b\b\b");
				printf("    ");
				printf("\b\b\b\b");
				if (arrow == 'A') {
					if (!s.empty()) {
						for (auto iter = cursor; iter != s.cbegin(); --iter) {
							printf("\b");
						}
                        for (auto iter = s.cbegin(); iter != s.cend(); ++iter) {
                            printf(" ");
                        }
						// printf("%s", string(s.size(), ' ').c_str());
						for (auto iter = s.cbegin(); iter != s.cend(); ++iter) {
							printf("\b");
						}
					}
					if (history.empty()) {
						s.clear();
						cursor = s.cbegin();
					}
					else if (cur == history.cbegin()) {
						cout << *cur;
						s = *cur;
						cursor = s.cend();
					}
					else {
						--cur;
						cout << *cur;
						s = *cur;
						cursor = s.cend();
					}
				}
				else if (arrow == 'B') {
					if (!s.empty()) {
						for (auto iter = cursor; iter != s.cbegin(); --iter) {
							printf("\b");
						}
                        for (auto iter = s.cbegin(); iter != s.cend(); ++iter) {
                            printf(" ");
                        }
						// printf("%s", string(s.size(), ' ').c_str());
						for (auto iter = s.cbegin(); iter != s.cend(); ++iter) {
							printf("\b");
						}
					}
					if (history.empty()) {
						s.clear();
						cursor = s.cbegin();
					}
					else if (cur == history.cend()) {
						s.clear();
						cursor = s.cbegin();
					}
					else if (cur == history.cend() - 1) {
						s.clear();
						cursor = s.cbegin();
					}
					else {
						++cur;
						cout << *cur;
						s = *cur;
						cursor = s.cend();
					}
				}
				else if (arrow == 'D') {
					// printf("\b");
					for (auto iter = cursor; iter != s.cend(); ++iter) {
						printf("%c", *iter);
					}
					for (auto iter = cursor; iter != s.cend(); ++iter) {
						printf("\b");
					}
					if (cursor != s.cbegin()) {
						--cursor;
						printf("\b");
					}
					// for (auto iter = cursor; iter != s.cend(); ++iter) {
					//     printf("%c", *iter);
					// }
				}
				else if (arrow == 'C') {
					if (cursor != s.cend()) {
						for (auto iter = cursor; iter != s.cend(); ++iter) {
							printf("%c", *iter);
						}
						++cursor;
						for (auto iter = cursor; iter != s.cend(); ++iter) {
							printf("\b");
						}
					}
				}
			}
			// backspace
			else if (c == 127) {
				if (cursor != s.cbegin()) {
					printf("\b\b\b");
					printf("   ");
					printf("\b\b\b");
					cursor = s.erase(--cursor);
					for (auto iter = cursor; iter != s.cend(); ++iter) {
						printf("%c", *iter);
					}
					printf(" ");
					for (auto iter = cursor; iter != s.cend(); ++iter) {
						printf("\b");
					}
					printf("\b");
				}
				else {
					printf("\b\b");
					printf("  ");
					printf("\b\b");
				}
			}
			else {
				cursor = s.insert(cursor, c);
				++cursor;
				for (auto iter = cursor; iter != s.cend(); ++iter) {
					printf("%c", *iter);
				}
				for (auto iter = cursor; iter != s.cend(); ++iter) {
					printf("\b");
				}
			}
		}
		if (c == EOF) {
			cout << endl;
			break;
		}
		system("/bin/stty cooked");
		// do {
		// 	if (std::getline(cin, s).eof()) {
		// 		cout << endl;
		// 		return 0;
		// 	}
		// } while (s.empty() || s.compare(string(s.size(), ' ')) == 0);
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
