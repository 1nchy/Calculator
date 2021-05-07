#include "display.h"

void DisplayErrorMsg(Expression &e, unsigned outPos) {
    auto dem = e.getErrorInfo();
    switch (dem.first) {
    case INVALID_SYNTAX:
        if (e.subfx.empty()) {
            cout << string(4+dem.second+outPos, ' ') << '^' << endl;
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