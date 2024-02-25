#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> myVector = {1, 2, 3, 4, 5};
    int targetValue = 3;

    auto it = std::find(myVector.begin(), myVector.end(), targetValue);

    if (it != myVector.end()) {
        // 元素找到
        std::cout << "Element found in the vector." << std::endl;
    } else {
        // 元素未找到
        std::cout << "Element not found in the vector." << std::endl;
    }

    return 0;
}
