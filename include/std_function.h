////
//// Created by 常鑫宇 on 2024/4/14.
////
//
//#ifndef STUDYCODE_STD_FUNCTION_H
//#define STUDYCODE_STD_FUNCTION_H
//
//#include <iostream>
//#include <functional>
//using namespace std;
//
////包装普通函数
//int g_Minus(int i, int j)
//{
//    return i - j;
//}
//
//int main()
//{
//    function<int(int, int)> f = g_Minus;
//    cout << f(1, 2) << endl;                                            // -1
//    return 1;
//}
//
////包装模板函数
//#include <iostream>
//#include <functional>
//using namespace std;
//
//template <class T>
//T g_Minus(T i, T j)
//{
//    return i - j;
//}
//
//int main()
//{
//    function<int(int, int)> f = g_Minus<int>;
//    cout << f(1, 2) << endl;                                            // -1
//    return 1;
//}
//
////包装lambda表达式
//#include <iostream>
//#include <functional>
//using namespace std;
//
//auto g_Minus = [](int i, int j){ return i - j; };
//
//int main()
//{
//    function<int(int, int)> f = g_Minus;
//    cout << f(1, 2) << endl;                                            // -1
//    return 1;
//}
//
////包装函数对象
//
//#include <iostream>
//#include <functional>
//using namespace std;
//
//struct Minus
//{
//    int operator() (int i, int j)
//    {
//        return i - j;
//    }
//};
//
//int main()
//{
//    function<int(int, int)> f = Minus();
//    cout << f(1, 2) << endl;                                            // -1
//    return 1;
//}
//
//
////模板类型：
//
//#include <iostream>
//#include <functional>
//using namespace std;
//
//template <class T>
//struct Minus
//{
//    T operator() (T i, T j)
//    {
//        return i - j;
//    }
//};
//
//int main()
//{
//    function<int(int, int)> f = Minus<int>();
//    cout << f(1, 2) << endl;                                            // -1
//    return 1;
//}
//
////- 包装类静态成员函数
////非模板类型
//#include <iostream>
//#include <functional>
//using namespace std;
//
//class Math
//{
//public:
//    static int Minus(int i, int j)
//    {
//        return i - j;
//    }
//};
//
//int main()
//{
//    function<int(int, int)> f = &Math::Minus;
//    cout << f(1, 2) << endl;                                            // -1
//    return 1;
//}
//
////模板类型
//#include <iostream>
//#include <functional>
//using namespace std;
//
//class Math
//{
//public:
//    template <class T>
//    static T Minus(T i, T j)
//    {
//        return i - j;
//    }
//};
//
//int main()
//{
//    function<int(int, int)> f = &Math::Minus<int>;
//    cout << f(1, 2) << endl;                                            // -1
//    return 1;
//}
//
//
////包装类对象成员函数
//
////非模板类型：
//#include <iostream>
//#include <functional>
//using namespace std;
//
//class Math
//{
//public:
//    int Minus(int i, int j)
//    {
//        return i - j;
//    }
//};
//
//int main()
//{
//    Math m;
//    function<int(int, int)> f = bind(&Math::Minus, &m, placeholders::_1, placeholders::_2);
//    cout << f(1, 2) << endl;                                            // -1
//    return 1;
//}
//
////模板类型
//#include <iostream>
//#include <functional>
//using namespace std;
//
//class Math
//{
//public:
//    template <class T>
//    T Minus(T i, T j)
//    {
//        return i - j;
//    }
//};
//
//int main()
//{
//    Math m;
//    function<int(int, int)> f = bind(&Math::Minus<int>, &m, placeholders::_1, placeholders::_2);
//    cout << f(1, 2) << endl;                                            // -1
//    return 1;
//}
//
//
//
//
//#endif //STUDYCODE_STD_FUNCTION_H
