#include "helpers.h"


class Boy {
public:
    Boy() {
        cout << "Boy 构造函数" << endl;
    }

    ~Boy() {
        cout << "~Boy 析构函数" << endl;
    }
};

class Girl {
public:
    Girl() {
        cout << "Girl 构造函数" << endl;
    }

    ~Girl() {
        cout << "~Girl 析构函数" << endl;
    }
};

class Person {
public:
    Person(int v) {
        this->no = v;
        cout << "构造函数 \t no = " << this->no << endl;
    }

    ~Person() {
        cout << "析构函数 \t no = " << this->no << endl;
    }

private:
    int no;
};

// 仿函数，内存删除
class DestructPerson {
public:
    void operator() (Person *pt) {
        cout << "DestructPerson..." << endl;
        delete pt;
    }
};



class CAutoPtr {

public:
    CAutoPtr(){}
    ~CAutoPtr(){}

    uint32_t unique_prt()
    {
        unique_ptr<string> p1(new string("I'm chang xin yu!"));
        unique_ptr<string> p2(new string("I'm age 25."));

        cout << "p1：" << p1.get() << endl;
        cout << "p2：" << p2.get() << endl;

        // p1 = p2;                   // 禁止左值赋值
        // unique_ptr<string> p3(p2); // 禁止左值赋值构造

        unique_ptr<string> p3(std::move(p1));
        p1 = std::move(p2); // 使用move把左值转成右值就可以赋值了，效果和auto_ptr赋值一样

        cout << "p1 = p2 赋值后：" << endl;
        cout << "p1：" << p1.get() << endl;
        cout << "p2：" << p2.get() << endl;

        return 0;
    }

    uint32_t unique_stl_ptr()
    {
        vector<unique_ptr<string>> vec;
        unique_ptr<string> p3(new string("I'm P3"));
        unique_ptr<string> p4(new string("I'm P4"));

        vec.push_back(std::move(p3));
        vec.push_back(std::move(p4));

        cout << "vec.at(0)：" << *vec.at(0) << endl;
        cout << "vec[1]：" << *vec[1] << endl;

//        vec[0] = vec[1];	/* 不允许直接赋值 */
        vec[0] = std::move(vec[1]);		// 需要使用move修饰，使得程序员知道后果

        cout << "vec.at(0)：" << *vec.at(0) << endl;
        cout << "vec[1]：" << *vec[1] << endl;

        return 0;
    }

    // 会自动调用delete [] 函数去释放内存
    //unique_ptr<int[]> array(new int[5]);	// 支持这样定义


    uint32_t auto_share_ptr()
    {
        shared_ptr<Person> sp1;

        shared_ptr<Person> sp2(new Person(2));

        // 获取智能指针管控的共享指针的数量	use_count()：引用计数
        cout << "sp1	use_count() = " << sp1.use_count() << endl;
        cout << "sp2	use_count() = " << sp2.use_count() << endl << endl;

        // 共享
        sp1 = sp2;

        cout << "sp1	use_count() = " << sp1.use_count() << endl;
        cout << "sp2	use_count() = " << sp2.use_count() << endl << endl;

        shared_ptr<Person> sp3(sp1);
        cout << "sp1	use_count() = " << sp1.use_count() << endl;
        cout << "sp2	use_count() = " << sp2.use_count() << endl;
        cout << "sp2	use_count() = " << sp3.use_count() << endl << endl;
    }


    void auto_weak_ptr()
    {
        shared_ptr<Boy> spBoy(new Boy());
        shared_ptr<Girl> spGirl(new Girl());

        // 弱指针的使用
        weak_ptr<Girl> wpGirl_1;			// 定义空的弱指针
        weak_ptr<Girl> wpGirl_2(spGirl);	// 使用共享指针构造
        wpGirl_1 = spGirl;					// 允许共享指针赋值给弱指针

        cout << "spGirl \t use_count = " << spGirl.use_count() << endl;
        cout << "wpGirl_1 \t use_count = " << wpGirl_1.use_count() << endl;


        // 弱指针不支持 * 和 -> 对指针的访问
        /*wpGirl_1->setBoyFriend(spBoy);
        (*wpGirl_1).setBoyFriend(spBoy);*/

        // 在必要的使用可以转换成共享指针
        shared_ptr<Girl> sp_girl;
        sp_girl = wpGirl_1.lock();

        cout << sp_girl.use_count() << endl;
        // 使用完之后，再将共享指针置NULL即可
        sp_girl = NULL;
    }
};