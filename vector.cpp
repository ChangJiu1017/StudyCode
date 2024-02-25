#include <iostream>
#include <map>

int main() {
    // 第一层 map 的 key 是 int，value 是第二层 map
    std::map<int, std::map<std::string, int>> twoLevelMap;

    // 添加一些数据
    twoLevelMap[1]["A"] = 10;
    twoLevelMap[1]["B"] = 20;
    twoLevelMap[2]["C"] = 30;

    // 遍历输出数据
    for (const auto& outerPair : twoLevelMap) {
        int firstKey = outerPair.first;
        const std::map<std::string, int>& innerMap = outerPair.second;

        for (const auto& innerPair : innerMap) {
            const std::string& secondKey = innerPair.first;
            int value = innerPair.second;

            std::cout << "First Key: " << firstKey << ", Second Key: " << secondKey << ", Value: " << value << std::endl;
        }
    }

    return 0;
}
