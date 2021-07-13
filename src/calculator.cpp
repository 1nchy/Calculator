#include "calculator.h"

unsigned Expression::next(string &s) {
    //跳过所有空白
    while (sci != expre.cend() && *sci == ' ') ++sci;
    if (sci == expre.cend() || *sci == '=') {
        if (*sci == '=') {
            _accurate = true;
        }
        return END_LINE;
    }
    mark = sci;
    auto c = *sci;
    bool dotFlag = false;
    static short base = 10;
    auto lambdaExp = [c](decltype(subst)::value_type im){return im.first[0] == c;};
    if (isDigitBegin(c)) {
        // 数字块
        flag = NUMBER;
        base = 10;
        if (c == '0') {
            if (sci + 1 != expre.cend()) {
                auto cnext = *(sci+1);
                auto btir = std::find_if(baseTable.cbegin(), baseTable.cend(), [cnext](decltype(baseTable)::value_type bv)->bool{return bv.first[1] == cnext;});
                if (btir != baseTable.cend()) {
                    base = btir->second;
                    sci += 2;
                    if (sci == expre.cend()) {
                        return INVALID_SYNTAX;
                    }
                    else {
                        c = *sci;
                        if (!is_digit(c, base)) {
                            return INVALID_SYNTAX;
                        }
                    }
                }
            }
        }
        while (1) {
            // 读取普通数字 /[0-9a-f\.]/
            if (!is_digit(c, base) && (c != '.' || dotFlag == true)) {
                break;
            }
            ++sci;
            if (c == '.') {
                dotFlag = true;
                if (sci == expre.cend() || !is_digit(*sci, base)) {
                    tinyErrorLocation += sci - mark;
                    flag = INVALID_SYNTAX;
                    return flag;
                }
            }
            if (sci == expre.cend()) {
                break;
            }
            c = *sci;
        }
        // 处理特殊符号 (e, pi, x)
        if (c == 'e') {
            ++sci;
        }
        else if (c == 'p') {
            ++sci;
            if (sci == expre.cend() || *sci != 'i') {
                tinyErrorLocation += sci - mark - 1;
                flag = INVALID_SYNTAX;
                return flag;
            }
            ++sci;
        }
        else if (c == independent_var) {
            ++sci;
        }
        if (sci != expre.cend() && isDigitBegin(*sci)) {
            return OPERATOR_LOST;
        }
    }
    else if (std::find_if(subst.cbegin(), subst.cend(), lambdaExp) != subst.cend()) {
        // 运算符块
        flag = INVALID_SYNTAX;
        ++sci;
        while (1) {
            if (c == '!') {
                // 处理双阶乘
                if (sci != expre.cend() && *sci == '!') {
                    ++sci;
                }
            }
            else if (c == '(') {
                if (sci != expre.cend() && *sci == ')') {
                    flag = OPERANDS_LOST;
                    break;
                }
            }
            string str(mark, sci);
            if (find_if(subst.cbegin(), subst.cend(), [str](decltype(subst)::value_type im){return im.first == str;}) != subst.cend()) {
                // 已成功匹配
                flag = OPERATOR;
                break;
            }
            if (sci == expre.cend()) {
                break;
            }
            c = *sci;
            if (!isalpha(c) && c != '!') {
                break;
            }
            ++sci;
        }
        // 判断正负号
        if (*mark == '+' || *mark == '-') {
            if (mark == expre.cbegin()) {
                flag = POSI_NEGA;
            }
            else {
                auto tmp = mark - 1;
                while (tmp != expre.cbegin() && *tmp == ' ') --tmp;
                // printf ("Current tmp: %c\n", *tmp);
                if ((tmp == expre.cbegin() && *tmp == ' ') || (!isDigitEnd(*tmp, base) && *tmp != ')' && *tmp != '!') ) {
                    flag = POSI_NEGA;
                }
            }
        }
    }
    else {
        flag = INVALID_SYNTAX;
    }
    s = string(mark, sci);
    return flag;
}

unsigned Expression::once() {
    if (symList.empty()) {
        return OPERATOR_LOST;
    }
    char c = symList.back();
    if (c == '(') {
        return PARENTHESES_LOST;
    }
    symList.pop_back();
    if (numList.empty()) {
        return OPERANDS_LOST;
    }
    double num1 = numList.back();
    numList.pop_back();
    if (numList.empty()) {
        return OPERANDS_LOST;
    }
    double num2 = numList.back();
    numList.pop_back();
    double ans = 0;
    auto f = count(num2, c, num1, ans);
    if (std::isnan(ans)) {
        return INVALID_INPUT;
    }
    if (f == CORRECT) {
        numList.push_back(ans);
    }
    return f;

}

unsigned Expression::count(double x, char c , double y, double& r) {
    switch (c) {
        case '+': r = x + y; break;
        case '-': r = x - y; break;
        case '*': r = x * y; break;
        case '/': r = x / y; break;
        case '^': r = std::pow(x, y); break;

        case 'l': if (!std::isnan(x)) return OPERATOR_LOST; r = std::log(y); break;
        case 's': if (!std::isnan(x)) return OPERATOR_LOST; r = std::sin(y); break;
        case 'c': if (!std::isnan(x)) return OPERATOR_LOST; r = std::cos(y); break;
        case 't': if (!std::isnan(x)) return OPERATOR_LOST; r = std::tan(y); break;
        case 'o': if (!std::isnan(x)) return OPERATOR_LOST; r = 1/std::tan(y); break;

        case 'S': if (!std::isnan(x)) return OPERATOR_LOST; r = std::asin(y); break;
        case 'D': if (!std::isnan(x)) return OPERATOR_LOST; r = std::acos(y); break;
        case 'T': if (!std::isnan(x)) return OPERATOR_LOST; r = std::atan(y); break;
        case 'O': if (!std::isnan(x)) return OPERATOR_LOST; r = -std::atan(y)+PI/2; break;

        case 'a': if (!std::isnan(x)) return OPERATOR_LOST; r = std::abs(y); break;

        case 'A': if (x<y||y<0) return INVALID_INPUT; r=parttimes((unsigned int)x, std::min((unsigned int)(x-y),(unsigned int)y)); break;
        case 'C': if (x<y||y<0) return INVALID_INPUT; r=parttimes((unsigned int)x, std::min((unsigned int)(x-y),(unsigned int)y))/fac(std::min((unsigned int)(x-y),(unsigned int)y)); break;
        case '%': if (y==0) return INVALID_INPUT; r = long(x) % long(y); break;
        case 'm': if (y==0) return INVALID_INPUT; r = (long(x) % long(y) + long(y)) % long(y); break;
        case 'i': if (x>=y||x<0) return INVALID_INPUT; r=invMod((unsigned int)x,(unsigned int)y); break;
        case '!': if (!std::isnan(y)) return OPERATOR_LOST; if (x<0) return INVALID_INPUT; r = fac((unsigned int)x); break;
        case '?': if (!std::isnan(y)) return OPERATOR_LOST; if (x<0) return INVALID_INPUT; r = profac((unsigned int)x); break;
        default: return INVALID_SYNTAX;
    }
    return CORRECT;
}

bool Expression::isDigitBegin(const char &c) {
    return (isdigit(c) || (c == 'p') || (c == 'e') || (c == independent_var));
}

bool Expression::isDigitEnd(const char &c, const short &base) {
    return (is_digit(c, base) || (c == 'i') || (c == 'e') || (c == independent_var));
}



unsigned Expression::compute() {
    if (computed) {
        return flag;
    }
    string s;
    double r;
    auto tmpSetFunc = this->setFunc;
    auto tmpAsgn = this->asgn;
    this->setFunc = false;
    this->asgn = false;
    this->computed = true;

    // 运算符和运算数入栈
    while (flag == CORRECT) {
        flag = next(s);
        if (flag == POSI_NEGA) {
            numList.push_back(0);
            symList.push_back(subst.at(s));
            flag = CORRECT;
        }
        else if (flag == NUMBER) {
            flag = dealWithNumber(s);
        }
        else if (flag == OPERATOR) {
            flag = dealWithOperator(s);
        }
        else if (flag == END_LINE) {
            break;
        }
    }
    if (flag != CORRECT && flag != END_LINE) {
        return flag;
    }
    while (!symList.empty()) {
        flag = once();
        if (flag != CORRECT) {
            return flag;
        }
    }
    if (numList.empty()) {
        flag = OPERANDS_LOST;
        return flag;
    }
    result = numList.back();
    if (std::isnan(result)) {
        flag = INVALID_INPUT;
        return flag;
    }
    numList.pop_back();
    if (!numList.empty()) {
        flag = OPERATOR_LOST;
        return flag;
    }
    if (tmpAsgn == true) {
        x = result;
    }
    if (tmpSetFunc == true) {
        this->fx = this->expre;
    }
    return CORRECT;
}

bool Expression::isMorePrior(const char &c) {
    if (symList.empty()) {
        return true;
    }
    char lastSym = symList.back();
    if (getPriority(c) > getPriority(lastSym)) {
        return true;
    }
    if (getPriority(c) == getPriority('+')) {
        return false;
    }
    else if (getPriority(c) == getPriority('*')) {
        return false;
    }
    else if (getPriority(c) == getPriority('!')) {
        return false;
    }
    else if (getPriority(c) == getPriority(lastSym)) {
        return true;
    }
    return false;
}

unsigned getPriority(const char &s) {
    switch (s) {
        case '(': case ')':
            return 0;
        case 'm': case 'i':
            return 1;
        case '+': case '-': 
            return 2;
        case '*': case '/': 
            return 3;
        case '^':
            return 4;
        case '%':
            return 5;
        case 'l':
            return 6;
        case 's': case 'c': case 't': case 'o': case 'S': case 'D': case 'T': case 'O':
            return 6;
        case 'A': case 'C':
            return 5;
        case 'a':
            return 6;
        case '!': case '?':
            return 9;
        default: 
            return 0;
    }
}

unsigned Expression::dealWithOperator(const string &s) {
    auto si = find_if(subst.cbegin(), subst.cend(), [s](decltype(subst)::value_type vt){return vt.first == s;});
    if (si == subst.cend()) {
        return INVALID_SYNTAX;
    }
    char readSym = si->second;
    if (readSym == '(') {
        symList.push_back(readSym);
    }
    else if (readSym == ')') {
        while (!symList.empty() && symList.back() != '(') {
            auto of = once();
            if (of != CORRECT) {
                return of;
            }
        }
        if (symList.empty()) {
            return PARENTHESES_LOST;
        }
        symList.pop_back();
    }
    else {
        if (isMorePrior(readSym)) {
            symList.push_back(subst.at(s));
            if (find(unaryOperator.cbegin(), unaryOperator.cend(), s) != unaryOperator.cend()) {
                numList.push_back(zero);
            }
        }
        else {
            do {
                auto of = once();
                if (of != CORRECT) {
                    return of;
                }
            } while (!isMorePrior(readSym));
            symList.push_back(subst.at(s));
            if (find(unaryOperator.cbegin(), unaryOperator.cend(), s) != unaryOperator.cend()) {
                numList.push_back(zero);
            }
        }
    }
    return CORRECT;
}

unsigned Expression::dealWithNumber(const string &s) {
    double r;
    if (tranStrToNum(s, r, x) == NUMBER) {
        numList.push_back(r);
        return CORRECT;
    }
    else {
        return INVALID_SYNTAX;
    }
}

unsigned tranStrToNum(const string &s, double &r, double x) {
    double integral = 0, fractional = 0;
    auto ir = s.cbegin();
    auto c = *ir;
    short base = 10;
    if (s.length() >= 2) {
        auto btir = std::find_if(baseTable.cbegin(), baseTable.cend(), [&s](decltype(baseTable)::value_type bt){return string(s.cbegin(), s.cbegin()+2) == bt.first;});
        if (btir != baseTable.cend()) {
            ir += 2;
            c = *ir;
            base = btir->second;
        }
    }
    if (c == 'p') {
        r = PI;
    }
    else if (c == 'e') {
        r = E;
    }
    else if (c == independent_var) {
        r = x;
    }
    else {
        // 整数部分
        while (1) {
            if (ir == s.cend()) {
                r = integral;
                return NUMBER;
            }
            c = *ir;
            if (!is_digit(c, base)) {
                break;
            }
            ++ir;
            integral = integral * base + static_cast<double>(get_digit(c, base));
        }
        if (c == 'p') {
            r = integral * PI;
        }
        else if (c == 'e') {
            r = integral * E;
        }
        else if (c == independent_var) {
            r = integral * x;
        }
        else {
            ++ir;
            for (int i = 1; 1; ++i) {
                if (ir == s.cend()) {
                    r = integral + fractional;
                    return NUMBER;
                }
                c = *ir;
                if (!is_digit(c, base)) {
                    break;
                }
                ++ir;
                fractional = fractional + static_cast<double>(get_digit(c, base)) * std::pow(1.0/base, i);
            }
            if (c == 'p') {
                r = (integral + fractional) * PI;
            }
            else if (c == 'e') {
                r = (integral + fractional) * E;
            }
            else if (c == independent_var) {
                r = (integral + fractional) * x;
            }
            else {
                return INVALID_SYNTAX;
            }
        }
    }
    return NUMBER;
}

std::pair<unsigned, unsigned> Expression::getErrorInfo() {
    unsigned error_pos = mark - expre.cbegin() + tinyErrorLocation;
    if (flag == INVALID_SYNTAX) {
        if (!subfx.empty()) {
            auto ir = std::find_if(subfx.cbegin(), subfx.cend(), [error_pos](decltype(subfx)::value_type fuck){return error_pos < fuck;});
            auto npos = (ir - subfx.cbegin()) * fx.size();
            if (ir != subfx.cbegin() && *(ir-1) + fx.size() + 2 >= error_pos) {
                npos -= fx.size() + error_pos - *(ir-1);
            }
            error_pos = error_pos - npos;
        }
    }
    return std::make_pair(flag, error_pos);
}
bool Expression::accurate() const {
    return _accurate;
}
double Expression::getx() const {
    return x;
}
void Expression::xzero() {
    x = 0;
}
string Expression::getfx() const {
    return fx;
}
void Expression::fxzero() {
    fx = "";
}
double Expression::getResult() {
    if (std::abs(result) < FLT_EPSILON) {
        result = 0;
    }
    return result;
}

string Expression::to_string(short base) {
    string s = "";
    std::ostringstream oss(s);
    if (base < 0 || base > 36) {
        return oss.str();
    }
    else if (base == 0) {
        oss << getResult();
        return oss.str();
    }
    else if (base == 1) {
        s = string_format("%.10lf", getResult());
        return s;
    }
    else {
        oss << convert(getResult(), base, FRAC_SIZE) << " (" << base << ")";
        return oss.str();
    }
}

string Expression::to_string(const string& base_str) {
    string s = "";
    std::ostringstream oss(s);
    if (base_str.empty()) {
        oss << getResult();
        return oss.str();
    }
    int base  = atoi(base_str.c_str());
    if (2 > base || base > 36) {
        oss << getResult() << " (10)\n" << "  :Base Not in Range";
        return oss.str();
    }
    else {
        oss << convert(getResult(), base, FRAC_SIZE) << " (" << base << ")";
        return oss.str();
    }
}

double fac(unsigned int n) {
    if (n <= 1) return 1;
    double r = 1;
    for (int i = 2; i <= n; ++i) {
        r *= i;
    }
    return r;
}
double profac(unsigned int n) {
    if (n<=1) return 1;
    double r = n;
    for (int i = n-2; i >= 2; i -= 2) {
        r *= i;
    }
    return r;
}
double parttimes(unsigned int x, unsigned int y) {
    double r = 1;
    for (int i = x; i > x-y; --i) {
        r *= i;
    }
    return r;
}
double invMod(unsigned int a, unsigned int m) {
    auto p = extEuclid(a,m);
    if (p.second != 1) {
        return 0;
    }
    else {
        return (p.first+m)%m;
    }
}
std::pair<unsigned int,unsigned int> extEuclid(unsigned int a, unsigned int b) {
    unsigned int x = 1, y = 0, m = a, n = b;
    if (b == 0) {
        return std::make_pair(x,a);
    }
    unsigned int x1 = 0, x2 = 1, y1 = 1, y2 = 0;
    while (n > 0) {
        unsigned int q = m / n;
        unsigned int r = m % n;
        m = n;
        n = r;
        x = x2 - q * x1;
        y = y2 - q * y1;
        x2 = x1; x1 = x;
        y2 = y1; y1 = y;
    }
    return std::make_pair(x2,m);
}
bool is_digit(const char &c, const short &base) {
    if (base <= 0 || base > 36) {
        return false;
    }
    else if (0 < base && base <= 10) {
        return '0' <= c && c < '0'+base;
    }
    else {
        return ('0' <= c && c <= '9') || ('a' <= c && c < 'a'+base-10) || ('A' <= c && c < 'A'+base-10);
    }
}
short get_digit(const char &c, const short &base) {
    if (base <= 0 || base > 36) {
        return -1;
    }
    else if (0 < base && base <= 10 && '0' <= c && c < '0'+base) {
        return c - '0';
    }
    else if ('0' <= c && c <= '9') {
        return c - '0';
    }
    else if ('a' <= c && c < 'a'+base-10) {
        return 10 + c - 'a';
    }
    else if ('A' <= c && c < 'A'+base-10) {
        return 10 + c - 'A';
    }
    else {
        return -1;
    }
}

void Expression::clear() {
    flag = CORRECT;
    tinyErrorLocation = 0;
    _accurate = false;
    numList.clear();
    symList.clear();
    asgn = false;
    computed = false;
}

void Expression::init(const string &s, const bool asgn, const bool asfx) {
    clear();
    subfx.clear();
    expre = s;
    auto pos = expre.find(string("fx"), 0);
    while (pos != string::npos) {
        subfx.push_back(pos);
        expre = expre.substr(0, pos) + string("(") + this->fx + string(")") + expre.substr(pos+2);
        pos = expre.find(string("fx"), pos + this->fx.size() + 1);
    }
    this->asgn = asgn;
    this->setFunc = asfx;
    mark = sci = expre.cbegin();
}

string convert(double d, short base, int frac_size) {
    string s = "";
    if (2 > base || base > 36) return s;
    bool posi = d >= 0;
    d = std::abs(d);
    do {
        int q = static_cast<int>(d/base);
        int r = static_cast<int>(d - q * base);
        d += q - static_cast<int>(d);
        char pb = r < 10 ? '0'+r : 'a'+r-10;
        s.insert(s.begin(), pb);
    } while (d >= 1);
    if (std::abs(d) > __FLT_EPSILON__ && frac_size) {
        // cout << "d = " << d << endl;
        s += '.';
    }
    for (int i = 0; std::abs(d) > __FLT_EPSILON__ && (frac_size >= 0 ? i < frac_size : 1); ++i) {
        int q = static_cast<int>(d * base);
        d = d * base - q;
        char pb = q < 10 ? '0'+q : 'a'+q-10;
        s.insert(s.end(), pb);
    }
    if (!posi) s.insert(s.begin(), '-');
    return s;
}

template<typename... Args> string string_format(const string& format, Args... args) {
    size_t size = 1 + snprintf(nullptr, 0, format.c_str(), args ...);  // Extra space for \0
    // unique_ptr<char[]> buf(new char[size]);
    char bytes[size];
    snprintf(bytes, size, format.c_str(), args ...);
    return string(bytes);
}
