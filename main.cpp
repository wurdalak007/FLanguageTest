#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <set>
#include "main.h"


uint32_t RegularAutomat::getMax() {
    findPrefix(0, 0);
    return maxP;
}

RegularAutomat::RegularAutomat(char _ch, const std::string& _pat) : pat(_pat), maxP(0) {
    Vertex root, tmp;
    size = 2;

    bohr.push_back(root);
    bohr.push_back(tmp);
    bohr[0].next[_ch] = 1;
    termVertex.insert(1);
}

void RegularAutomat::Closer() {
    for (auto elem : termVertex) {
        bohr[elem].next['1'] = 0;
    }
    termVertex.insert(0);
}

void RegularAutomat::findPrefix(int verNum, int startPos) {
    if (startPos > static_cast<int>(pat.length()))
        return;

    if (termVertex.count(verNum) != 0 && startPos >= maxP) maxP = startPos;

    for (auto el : bohr[verNum].next) {
        if (el.first == '1') {
            if (el.second != 0 || startPos != 0)
                findPrefix(el.second, startPos);
        } else if (el.first == pat[startPos]){
            findPrefix(el.second, startPos+1);
        }
    }
}

void add(std::stack<RegularAutomat>& st) {
    try {
        RegularAutomat first = st.top();
        st.pop();
        RegularAutomat sec = st.top();
        st.pop();
        first = first + sec;
        st.push(first);
    } catch (char *err) {
        std::cout << "Something went wrong... in add";
    }

}

void mul(std::stack<RegularAutomat>& st) {
    try {
        RegularAutomat first = st.top();
        st.pop();
        RegularAutomat sec = st.top();
        st.pop();
        first = sec * first;
        st.push(first);
    } catch (char* err) {
        std::cout << "Something went wrong... in mul";
    }

}

int main() {
    std::stack<RegularAutomat> res;
    std::string source, pattern;
    std::cin >> source >> pattern;

    for (auto ch : source) {
        if ( (ch >= 'a' && ch <= 'c') || ch == '1') {
            res.push(RegularAutomat(ch, pattern));
        } else {
            switch (ch) {
                case '+':
                    add(res);
                    break;
                case '.':
                    mul(res);
                    break;
                case '*':
                    res.top().Closer();
                    break;

                default:
                    std::cout << "Incorrect symbol";
                    exit(0);
                    break;
            }
        }
    }

    RegularAutomat result = res.top();
    uint32_t len = result.getMax();

    if (len)
        for (int i = 0; i < len; i++) {
            std::cout << pattern[i];
        }
    else
        std::cout << "There is no prefix";

    return 0;
}


