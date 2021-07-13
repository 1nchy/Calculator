#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <list>
#include <algorithm>
#include <cmath>
#include <cctype>
#include <cfloat>

#define CORRECT 0
#define NUMBER 1
#define OPERATOR 2
#define END_LINE 3			// 结束
#define INVALID_SYNTAX 4
#define INVALID_INPUT 5
#define ZERO_DIVISION 6
#define PARENTHESES_LOST 7
#define OPERANDS_LOST 8
#define OPERATOR_LOST 9
#define UNKNOWN_ERROR 10
#define POSI_NEGA 32		// 正负号

// #define INDEPENDENT_VARIABLE 'x'

#define PI 3.141592653589793
#define E 2.718281828459045

#define FRAC_SIZE 50

using std::string;
using std::vector;
using std::map;
using std::set;
using std::list;
// 替换表
const map<string, char> subst {
	{string("("), '('}, {string(")"), ')'},
	{string("+"), '+'}, {string("-"), '-'},
	{string("*"), '*'}, {string("/"), '/'},
	{string("^"), '^'}, {string("%"), '%'},
	{string("sin"), 's'}, {string("cos"), 'c'},
	{string("tan"), 't'}, {string("cot"), 'o'},
	{string("asin"), 'S'}, {string("acos"), 'D'},
	{string("atan"), 'T'}, {string("acot"), 'O'},
	{string("ln"), 'l'},
	{string("abs"), 'a'},
	{string("!"), '!'}, {string("!!"), '?'},
	{string("C"), 'C'}, {string("A"), 'A'},
	{string("mod"), 'm'}, {string("inv"), 'i'},
	{string("pi"), 'p'}, {string("e"), 'e'}
};
// 一元运算符
const set<string> unaryOperator {
	string("sin"), 	string("cos"),
	string("tan"), 	string("cot"),
	string("ln"),
	string("abs"),
	string("!"), string("!!")
};
// 进制转换表
const map<string, short> baseTable {
	{string("0b"), 2}, {string("0B"), 2},
	{string("0x"), 16}, {string("0X"), 16}
};
const double zero = std::sqrt(-1);

const char asgnCmd = ':';
const char funcCmd = '\\';
const string baseTrans = "->";
const char independent_var = 'x';

// const int frac_size = 50;

class Expression {
private:
	list<double> numList;
	list<char> symList;
	string::const_iterator sci, mark;
	bool _accurate;
	string expre;
	bool asgn = false;
	bool setFunc = false;
	double x = 0;
	string fx = "";
	double result = 0;
	bool computed;
	vector<size_t> subfx;
	unsigned flag;

	void clear();
	unsigned once();
	unsigned count(double, char, double, double&);
	unsigned dealWithOperator(const string&);
	unsigned dealWithNumber(const string&);
	unsigned tinyErrorLocation;
	inline bool isDigitBegin(const char &c);
	inline bool isDigitEnd(const char &c, const short &base);
	unsigned next(string &s);
	// 判断当前运算符优先级是否比栈顶运算符优先级更高
	bool isMorePrior(const char &s);
public:
	Expression() {};
	void init(const string &s, const bool asgn, const bool asfx);
	unsigned compute();
	bool accurate() const;
	std::pair<unsigned, unsigned> getErrorInfo();
	double getx() const;
	void xzero();
	string getfx() const;
	void fxzero();
	double getResult();
	string to_string(short = 0);
	string to_string(const string& = "");
};

unsigned tranStrToNum(const string&, double &r, double x);
unsigned getPriority(const char &s);

double fac(unsigned int n);
double profac(unsigned int n);
double parttimes(unsigned int x, unsigned int y);
double invMod(unsigned int a, unsigned int m);
std::pair<unsigned int,unsigned int> extEuclid(unsigned int a, unsigned int b);
bool is_digit(const char &c, const short &base);
short get_digit(const char &c, const short &base);
string convert(double, short, int = -1);

template<typename... Args> string string_format(const string& format, Args... args);

#endif
