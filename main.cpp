#include <iostream>
#include <algorithm>

#include "calculator.h"
#include "display.h"

using std::cin;
using std::cout;
using std::endl;

using std::find;

const string welcomeMsg = "Calculator by 0nchy, enjoy yourself ~";
const string versionMsg = "Version 3.2.8 on linux";

inline void backspace(int n) {
    for (int i = 0; i < n; ++i) printf("\b");
}
inline void ctrlh(int n) {
    backspace(n);
    printf("%*s", n, "");
    backspace(n);
}

int main(int argc, char *argv[]) {
	cout << welcomeMsg << endl;
	cout << versionMsg << endl;
	Expression ep;
	string function;
	vector<string> history;
	vector<string>::const_iterator cur;
	vector<char> front;
	vector<char> behind;
	while (1) {
		printf("%s", inputNotice.c_str());
		bool asgn = false, asfx = false;
		unsigned outPos = 0;
		string s;
		int c;
		cur = history.cend();
        front.clear();
        behind.clear();
        system("stty raw");
        while ((c = getchar()) != EOF) {
            if (c == '\r' || c == '\n') {
                ctrlh(2);
                if (front.empty() && behind.empty()) {
                    continue;
                }
                s = string(front.cbegin(), front.cend()) + string(behind.crbegin(), behind.crend());
                if (s.empty() || s.compare(string(s.size(), ' ')) == 0) {
                    continue;
                }
                std::for_each(behind.crbegin(), behind.crend(), [](char ch){printf("%c", ch);});
                system("stty cooked");
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
            // combined key
            else if (c == '\033') { // c == '^['
				getchar(); // read a '['
				int arrow = getchar();
                // up arrow "^[[A"
                if (arrow == 'A') {
                    ctrlh(4);
                    backspace(front.size());
                    printf("%*s", front.size() + behind.size(), "");
                    backspace(front.size() + behind.size());
                    if (history.empty()) {
                        front.clear();
                        behind.clear();
                    }
                    else if (cur == history.cbegin()) {
                        cout << *cur;
                        front.clear();
                        front.assign(cur->cbegin(), cur->cend());
                        behind.clear();
                    }
                    else {
                        --cur;
                        cout << *cur;
                        front.clear();
                        front.assign(cur->cbegin(), cur->cend());
                        behind.clear();
                    }
                }
                // down arrow
                else if (arrow == 'B') {
                    ctrlh(4);
                    backspace(front.size());
                    printf("%*s", front.size() + behind.size(), "");
                    backspace(front.size() + behind.size());
                    if (history.empty()) {
                        front.clear();
                        behind.clear();
                    }
                    else if (cur == history.cend()) {
                        front.clear();
                        behind.clear();
                    }
                    else if (cur == history.cend() - 1) {
                        front.clear();
                        behind.clear();
                    }
                    else {
                        ++cur;
                        cout << *cur;
                        front.clear();
                        front.assign(cur->cbegin(), cur->cend());
                        behind.clear();
                    }
                }
                // left arrow
                else if (arrow == 'D') {
                    ctrlh(4);
                    std::for_each(behind.crbegin(), behind.crend(), [](char ch){printf("%c", ch);});
                    backspace(behind.size());
                    if (!front.empty()) {
                        behind.push_back(front.back());
                        front.pop_back();
                        printf("\b");
                    }
                }
                // right arrow
                else if (arrow == 'C') {
                    ctrlh(4);
                    if (!behind.empty()) {
                        std::for_each(behind.crbegin(), behind.crend(), [](char ch){printf("%c", ch);});
                        front.push_back(behind.back());
                        behind.pop_back();
                        backspace(behind.size());
                    }
                }
                // home key "^[[H"
                else if (arrow == 'H') {
                    ctrlh(4);
                    std::for_each(behind.crbegin(), behind.crend(), [](char ch){printf("%c", ch);});
                    behind.insert(behind.end(), front.crbegin(), front.crend());
                    front.clear();
                    backspace(behind.size());
                }
                // end key "^[[F"
                else if (arrow == 'F') {
                    ctrlh(4);
                    std::for_each(behind.crbegin(), behind.crend(), [](char ch){printf("%c", ch);});
                    front.insert(front.end(), behind.crbegin(), behind.crend());
                    behind.clear();
                }
                // delete key "^[[3~"
                else if (arrow == '3') {
                    getchar(); // read a '~'
                    if (!behind.empty()) {
                        ctrlh(5);
                        behind.pop_back();
                        std::for_each(behind.crbegin(), behind.crend(), [](char ch){printf("%c", ch);});
                        printf(" ");
                        backspace(behind.size() + 1);
                    }
                    else {
                        ctrlh(5);
                    }
                }
            }
            // backspace
            else if (c == 127) {
                if (!front.empty()) {
                    ctrlh(3);
                    front.pop_back();
                    std::for_each(behind.crbegin(), behind.crend(), [](char ch){printf("%c", ch);});
                    printf(" ");
                    backspace(behind.size() + 1);
                }
                else {
                    ctrlh(2);
                }
            }
            else {
                // prohibit typing spaces at the beginning
                if (c == ' ' && front.empty()) {
                    printf("\b");
                    if (!behind.empty()) {
                        printf("%c", behind.back());
                        printf("\b");
                    }
                    continue;
                }
                front.push_back(c);
                std::for_each(behind.crbegin(), behind.crend(), [](char ch){printf("%c", ch);});
                backspace(behind.size());
            }
        }
        if (c == EOF) {
            cout << endl;
            break;
        }
        system("stty cooked");
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
