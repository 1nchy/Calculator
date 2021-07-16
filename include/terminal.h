#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::for_each;

class terminal {
private:
    vector<string> history;
    vector<string>::const_iterator cur;
    vector<char> front;
    vector<char> behind;
    string notice;
    inline void backspace(int n) const;
    inline void ctrlh(int n) const;
    bool end_of_file;
public:
    string wait(const string& = "");
    string last() const;
    string get_notice() const;
    bool eof() const;
    terminal(const string& = "");
    terminal(const terminal&)=delete;
    terminal& operator=(const terminal&)=delete;
};