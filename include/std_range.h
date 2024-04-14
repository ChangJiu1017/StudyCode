//
// Created by 常鑫宇 on 2024/4/14.
//

#ifndef STUDYCODE_STD_RANGE_H
#define STUDYCODE_STD_RANGE_H

#include "helpers.h"

void range_traditional()
{
    vector<int> v1 = {0, 1, 2, 3, 4};
    vector<int> v2;
    for (auto i : v1) {
        if (i % 2 == 0) {
            v2.push_back(i * i);
        }
    }
    vector<int> v3(v2.rbegin(), v2.rend());
}

void range_channal()
{
    for (int i : v1 | views::filter([](int i) { return i % 2 == 0; })
                 | views::transform([](int i) { return i * i; })
                 | views::reverse) {
        printf("%d ", i);
    }
}





#endif //STUDYCODE_STD_RANGE_H
