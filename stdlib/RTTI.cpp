#include <iostream> 
#include <typeinfo> 

using namespace std; 

class Base { 
public: 
    virtual void foo() { 
        cout << "foo" << endl; 
    } 
}; 

class Derived1 : public Base { 
}; 

class Derived2 : public Base { 
}; 

void printTypeInfo(Base *base) {
    const type_info &info = typeid(*base);
    cout << info.name()      << "\t"
         << info.hash_code() << endl;     
}

typedef void (*FuncPtr)();
int test()
{
    XX xx;
    FuncPtr func;
    char *p = (char*)&xx;
    // 获得虚函数表的地址
    int **vtbl = (int**)*(int**)p;
    // 输出虚函数表的地址，即vptr的值
    cout << vtbl << endl;
    // 获得type_info对象的指针，并调用其name成员函数
    cout << "\t[-1]: " << (vtbl[-1]) << " -> "
        << ((type_info*)(vtbl[-1]))->name() << endl;
    // 调用第一个virtual函数
    cout << "\t[0]: " << vtbl[0] << " -> ";
    func = (FuncPtr)vtbl[0];
    func();
    // 输出基类的成员变量的值
    p += sizeof(int**);
    cout << *(int*)p << endl;
    // 输出派生类的成员变量的值
    p += sizeof(int);
    cout << *(int*)p << endl;
    return 0;
}

int main() { 
    Base *derived1 = new Derived1();
    Base *derived2 = new Derived2();

    printTypeInfo(derived1);
    printTypeInfo(derived2);

    return 0;
}

// class type_info
// {
//     public:
//         virtual ~type_info();
//         bool operator==(const type_info&)const;
//         bool operator!=(const type_info&)const;
//         bool before(const type_info&)const;
//         const char* name()const;
//     private:
//         type_info(const type_info&);
//         type_info& operator=(const type_info&);
       
//         // data members
// };