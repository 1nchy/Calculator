#include "terminal.h"

void terminal::backspace(int n) const {
	for (int i = 0; i < n; ++i) printf("\b");
}
void terminal::ctrlh(int n) const {
	backspace(n);
	printf("%*s", n, "");
	backspace(n);
}

string terminal::wait(const string& _notice) {
	if (_notice.empty()) {
		printf("%s ", this->notice.c_str());
	}
	else {
		printf("%s ", _notice.c_str());
	}
    string s;
	int c;
	front.clear();
	behind.clear();
    end_of_file = false;
    cur = history.cend();
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
            end_of_file = true;
			return "";
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
	system("stty cooked");
	if (c == EOF) {
        end_of_file = true;
		cout << endl;
	}
    return s;
}

string terminal::last() const {
    if (history.empty()) {
        return "";
    }
    else {
        return history.back();
    }
}

string terminal::get_notice() const {
    return notice;
}

bool terminal::eof() const {
    return end_of_file;
}

terminal::terminal(const string& _notice): notice(_notice), end_of_file(false) {
	history.clear();
    front.clear();
    behind.clear();
    cur = history.cend();
}
