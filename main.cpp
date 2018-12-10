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

RegularAutomat build (std::string source, std::string pattern, int& errorCode) {
    std::stack<RegularAutomat> res;
    
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
                    // errorCode == 1 is Incorrect symbol;
                    errorCode = 1;
                    return RegularAutomat('e', "error");
            }
        }
    }
    
    RegularAutomat result = res.top();
    errorCode = 0;
    if (res.size() != 1) {
        errorCode = -1; // Incorrect string
    }
    
    return result;
}



int main() {
    if (!testing()) {
        std::cout << "FAILED TESTING";
        return 0;
    } else {
        std::cout << "SUCCEED TESTING";
    }
    
    
    int error = 0; // OK
    std::string source, pattern;
    std::cin >> source >> pattern;
    RegularAutomat result = build(source, pattern, error);
    
    uint32_t len = result.getMax();
    
    std::cout << len;
    
    return 0;
}


bool testing () {
    bool passed = true;
    int error = 0;
    std::string source = "ab+c.aba.*.bac.+.+*";
    std::string pattern = "abacb";
    uint32_t answer = 4;
    
    RegularAutomat result = build(source, pattern, error);
    
    uint32_t len = result.getMax();
    std::cout << "FIRST TEST - " << source + " " + pattern + "\n";
    if (error == 0 && len == answer) {
        std::cout << "FIRST test passed, answer is " << len << "\n";
    } else {
        std::cout << "FIRST test failed, your answer is " << len << " , correct answer is " << answer << "\n";
        passed = false;
    }
    
    source = "acb..bab.c.*.ab.ba.+.+*a.";
    pattern = "cb";
    answer = 0;
    result = build(source, pattern, error);
    len = result.getMax();
    std::cout << "SECOND TEST - " << source + " " + pattern + "\n";
    if (error == 0 && len == answer) {
        std::cout << "SECOND test passed, answer is " << len << "\n";
    } else {
        std::cout << "SECOND test failed, your answer is " << len << " , correct answer is " << answer << "\n";
        passed = false;
    }
    
    source = "afd+.+*a.";
    pattern = "cb";
    result = build(source, pattern, error);
    std::cout << "THIRD TEST - " << source + " " + pattern + "\n";
    if (error == 1) {
        std::cout << "THIRD test passed, answer is " << "Incorrect symbol" << "\n";
    } else {
        std::cout << "THIRD test failed, " << "EXPECTED - Incorrect symbol";
        passed = false;
    }
    
    source = "abcabc*";
    pattern = "cb";
    result = build(source, pattern, error);
    std::cout << "FOURTH TEST - " << source + " " + pattern + "\n";
    if (error == -1) {
        std::cout << "FOURTH test passed, answer is " << "Incorrect string" << "\n";
    } else {
        std::cout << "FOURTH test failed, " << "EXPECTED - Incorrect string";
        passed = false;
    }
    
    source = "ab+*";
    pattern = "abbabb";
    answer = 6;
    result = build(source, pattern, error);
    len = result.getMax();
    std::cout << "FIFTH TEST - " << source + " " + pattern + "\n";
    if (error == 0 && len == answer) {
        std::cout << "FIFTH test passed, answer is " << len << "\n";
    } else {
        std::cout << "FIFTH test failed, your answer is " << len << " , correct answer is " << answer << "\n";
        passed = false;
    }
    
    source = "abbac+*++.";
    pattern = "acccabac";
    answer = 4;
    result = build(source, pattern, error);
    len = result.getMax();
    std::cout << "SIXTH TEST - " << source + " " + pattern + "\n";
    if (error == 0 && len == answer) {
        std::cout << "SIXTH test passed, answer is " << len << "\n";
    } else {
        std::cout << "SIXTH test failed, your answer is " << len << " , correct answer is " << answer << "\n";
        passed = false;
    }
    
    source = "ab+";
    pattern = "ababc";
    answer = 1;
    result = build(source, pattern, error);
    len = result.getMax();
    std::cout << "SEVENTH TEST - " << source + " " + pattern + "\n";
    if (error == 0 && len == answer) {
        std::cout << "SEVENTH test passed, answer is " << len << "\n";
    } else {
        std::cout << "SEVENTH test failed, your answer is " << len << " , correct answer is " << answer << "\n";
        passed = false;
    }
    
    source = "ab.";
    pattern = "ababc";
    answer = 2;
    result = build(source, pattern, error);
    len = result.getMax();
    std::cout << "EIGHTS TEST - " << source + " " + pattern + "\n";
    if (error == 0 && len == answer) {
        std::cout << "EIGHTS test passed, answer is " << len << "\n";
    } else {
        std::cout << "EIGHTS test failed, your answer is " << len << " , correct answer is " << answer << "\n";
        passed = false;
    }
    
    source = "ab.";
    pattern = "ba";
    answer = 0;
    result = build(source, pattern, error);
    len = result.getMax();
    std::cout << "NINETH TEST - " << source + " " + pattern + "\n";
    if (error == 0 && len == answer) {
        std::cout << "NINETH test passed, answer is " << len << "\n";
    } else {
        std::cout << "NINETH test failed, your answer is " << len << " , correct answer is " << answer << "\n";
        passed = false;
    }
    
    
    source = "ab*";
    pattern = "cb";
    result = build(source, pattern, error);
    std::cout << "TENTH TEST - " << source + " " + pattern + "\n";
    if (error == -1) {
        std::cout << "TENTH test passed, answer is " << "Incorrect string" << "\n";
    } else {
        std::cout << "TENTH test failed, " << "EXPECTED - Incorrect string";
        passed = false;
    }
    
    return passed;
}
