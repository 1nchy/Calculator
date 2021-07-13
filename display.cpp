#include "display.h"

void DisplayErrorMsg(const std::pair<unsigned, unsigned> &dem, unsigned outPos) {
    switch (dem.first) {
    case INVALID_SYNTAX:
        cout << string(inputNotice.size()+1+dem.second+outPos, ' ') << '^' << endl;
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
