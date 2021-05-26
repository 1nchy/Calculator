#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include <iostream>
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

#define INDEPENDENT_VARIABLE 'x'

#define PI 3.141592653589793
#define E 2.718281828459045

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
	{string("mod"), 'm'},
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
const double zero = std::sqrt(-1);

class Expression {
private:
	list<double> numList;
	list<char> symList;
	string::const_iterator sci, mark;
	unsigned once();
	unsigned count(double, char, double, double&);
	unsigned dealWithOperator(const string&);
	unsigned dealWithNumber(const string&);
	unsigned tranStrToNum(const string&, double &r);
	unsigned tinyErrorLocation;
	inline bool isDigitBegin(const char &c);
	inline bool isDigitEnd(const char &c);
	unsigned next(string &s);
	// 判断当前运算符优先级是否比栈顶运算符优先级更高
	bool isMorePrior(const char &s);
	bool setFunc = false;
	void clear();
public:
	Expression() {};
	void init(const string &s, const bool asgn, const bool asfx);
	string expre;
	string fx = "";
	unsigned compute();
	unsigned flag;
	bool accurate;
	bool asgn = false;
	double result = 0;
	double x = 0;
	vector<size_t> subfx;
	std::pair<unsigned, unsigned> getErrorInfo();
	double getResult();
	
};

unsigned getPriority(const char &s);

double fac(int n);
double profac(int n);
double parttimes(int x, int y);

#endif
