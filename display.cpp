#include "display.h"

void DisplayErrorMsg(Expression &e, unsigned outPos) {
    auto dem = e.getErrorInfo();
    switch (dem.first) {
    case INVALID_SYNTAX:
        if (e.subfx.empty()) {
            cout << string(inputNotice.size()+1+dem.second+outPos, ' ') << '^' << endl;
        }
        else {
            auto ir = find_if(e.subfx.cbegin(), e.subfx.cend(), [dem](decltype(e.subfx)::value_type fuck){return dem.second < fuck;});
            auto nPos = (ir - e.subfx.cbegin()) * e.fx.size();
            // 下面的条件可能已经无用了(永远不会到该分支), 但以防意外未删除
            if (ir != e.subfx.cbegin() && *(ir - 1) + e.fx.size() + 2 >= dem.second) {
                nPos = nPos - e.fx.size() + (dem.second - *(ir - 1));
            }
            cout << string(4+dem.second+outPos-nPos, ' ') << '^' << endl;
        }
        cout << "  :INVALID_SYNTAX" << endl;
        break;
    case INVALID_INPUT:
        cout << "  :INVALID_INPUT" << endl;
        break;
    case ZERO_DIVISION:
        cout << "  :ZERO_DIVISION" << endl;
        break;
    case PARENTHESES_LOST:
        cout << "  :PARENTHESES_LOST" << endl;
        break;
    case OPERANDS_LOST:
        cout << "  :OPERANDS_LOST" << endl;
        break;
    case OPERATOR_LOST:
        cout << "  :OPERATOR_LOST" << endl;
    break;
    case UNKNOWN_ERROR:
        cout << "  :UNKNOWN_ERROR" << endl;
        break;
    default: break;
    }
}

void DisplayOperators() {
    std::for_each(subst.cbegin(), subst.cend(), [](decltype(subst)::value_type vs){cout << vs.first << endl;});
}

void DisplayResult(const double &r, const string &base_str) {
    if (base_str.empty()) {
        cout << r << endl;
        return;
    }
    int base = atoi(base_str.c_str());
    // cout << "--" << base << endl;
    if (2 > base || base > 36) {
        cout << r << " (10)\n" << "  :Base Not in Range" << endl;
    }
    else {
        cout << convert(r, base) << " (" << base << ")" << endl;
    }
}

string convert(double d, short base) {
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
    if (std::abs(d) > __FLT_EPSILON__) {
        // cout << "d = " << d << endl;
        s += '.';
    }
    while (std::abs(d) > __FLT_EPSILON__) {
        int q = static_cast<int>(d * base);
        d = d * base - q;
        char pb = q < 10 ? '0'+q : 'a'+q-10;
        s.insert(s.end(), pb);
    }
    if (!posi) s.insert(s.begin(), '-');
    return s;
}
