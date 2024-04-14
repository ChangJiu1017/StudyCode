//
// Created by 常鑫宇 on 2024/4/14.
//

#ifndef STUDYCODE_CALL_BACK_FUNCTION_H
#define STUDYCODE_CALL_BACK_FUNCTION_H

#include <iostream>
#include <vector>
#include <algorithm>

void print(int i) {
    std::cout << i << " ";
}

void forEach(const std::vector<int>& v, const void(*callback)(int)) {
    for(auto i : v) {
        callback(i);
    }
}

int main() {
    std::vector<int> v = {1,2,3,4,5};
    forEach(v, [](int i){std::cout << i << " ";});

    return 0;
}


#endif //STUDYCODE_CALL_BACK_FUNCTION_H
