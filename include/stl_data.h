#pragma once

#include "helpers.h"

class CStlData {

public:
    CStlData(){}
    ~CStlData(){}

    uint32_t vector_stl()
    {
        // 第一层 map 的 key 是 int，value 是第二层 map
        std::map<int, std::map<std::string, int>> twoLevelMap;

        // 添加一些数据
        twoLevelMap[1]["A"] = 10;
        twoLevelMap[1]["B"] = 20;
        twoLevelMap[2]["C"] = 30;

        // 遍历输出数据
        for (const auto &outerPair : twoLevelMap)
        {
            int firstKey = outerPair.first;
            const std::map<std::string, int> &innerMap = outerPair.second;

            for (const auto &innerPair : innerMap)
            {
                const std::string &secondKey = innerPair.first;
                int value = innerPair.second;

                std::cout << "First Key: " << firstKey << ", Second Key: " << secondKey << ", Value: " << value << std::endl;
            }
        }
        return 0;
    }

    uint32_t map_stl()
    {
        std::vector<int> myVector = {1, 2, 3, 4, 5};
        int targetValue = 3;

        auto it = std::find(myVector.begin(), myVector.end(), targetValue);

        if (it != myVector.end())
        {
            // 元素找到
            std::cout << "Element found in the vector." << std::endl;
        }
        else
        {
            // 元素未找到
            std::cout << "Element not found in the vector." << std::endl;
        }

        return 0;
    }
};