//
// Created by hsl19 on 2026/7/9.
//
#include <iostream>
#include <type_traits>
using namespace std;

template<typename T>
T max_(T a, T b){
    return a > b ? a : b;
}


template<typename T>
class vector_{
    T* data;
};



// 模板参数的分类
// 类型参数
// 非类型参数 int N，将值提升到了编译期，使值能够参与模板实例化，类型系统和编译器优化
// 模板模板参数








// --- 关于完美转发以及四种引用折叠 ---
// 什么是完美转发
void func(int& x){
    std::cout << "lvalue" << std::endl;
}

void func(int&& x){
    std::cout << "rvalue" << std::endl;
}

// 你想写一个包装器
template<typename T>
void wrapper(T&& x){// 引用折叠推导，如果x是左值，那么T推导为T&，如果x是右值，那么T推导为x的类型
//    func(x);// 只要对象有名字，那就是左值
    func(std::forward<T>(x));// 完美转发，本质也不是运行时判断，而是编译时的推导，内部使用一个static_cast
    // Q：为什么非要用函数模板呢？因为会丢失value category吧，也就是通过& &&定义的形参只能将实参的值类别保留一层调用。
    // 不用模板参数的话，就只能用两个& 和 && 的函数了，现在却能一个函数解决。
    // 值类别只能跨越一层
}


void test_remain_value_category(){
    int a = 10;
    wrapper(a);
    wrapper(20);
}





// --- 类型萃取 TMP 元函数 ---
// 本质上类型萃取就是TMP的元函数，是一个特殊的结构体
// 类型萃取，type_traits，是一种编译时技术，用于在编译期间获取和操作类型的信息
// cpp引入了<type_traits>头文件，包含许多内置类型的萃取
// std::is_integral<T>,判断T是否为整数类型
// std::is_floating_point<T>，判断类型是否为浮点数类型
// std::is_pointer<T>，判断类型是否为指针类型
// std::is_reference<T>，判断是否为引用类型
// std::is_const<T>，判断是否为const类型
// std::is_same<T,U>，判断T和U是否相同

// true_type以及 false_type
// 1. 基础模板：将任意整数常量包装为一个类型
//template <typename T, T v>
//struct integral_constant {
//    static constexpr T value = v;
//    using value_type = T;
//    using type = integral_constant;
//
//    // 隐式转换为原始数据类型
//    constexpr operator value_type() const noexcept { return value; }
//    // 重载调用运算符，方便像函数一样调用：std::true_type{}() -> true
//    constexpr value_type operator()() const noexcept { return value; }
//};
//
// 2. 特化定义：bool 常量类型的别名
//using true_type  = integral_constant<bool, true>;
//using false_type = integral_constant<bool, false>;

template <typename T>
struct is_integral_helper : std::false_type {};

template <>
struct is_integral_helper<bool> : std::true_type {};

template <>
struct is_integral_helper<char> : std::true_type {};

template <>
struct is_integral_helper<short> : std::true_type {};

template <>
struct is_integral_helper<unsigned short> : std::true_type {};

template <>
struct is_integral_helper<int> : std::true_type {};

// 这里定义is_integral 为is_integral_helper的一个包装，首先移除给定类型的const和volatile限定符
template <typename T>
struct is_integral : is_integral_helper<typename std::remove_cv<T>::type> {};

// 应用
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
foo(T t) {
    std::cout << "foo() called with an integral type: " << t << std::endl;
    return t;
}

// 如果为false相当于不存在
template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type
foo(T t) {
    std::cout << "foo() called with a floating point type: " << t << std::endl;
    return t;
}




// 变长的模板参数，这是cpp11引入的
// 变长模板参数的应用？
// 类型安全的格式化与日志系统，
// 变长模板参数的解包？
template<typename... Args>
void func(Args... args){
    std::cout << sizeof...(args) << "\n";
}

/**
 *
 * 系统学习cpp的模板
 * 有函数模板
 * 类模板
 * 模板实例化
 * 模板参数
 * 默认模板参数
 * 偏特化，甚至还有指针类型的偏特化
 * 模板函数不能偏特化，为了避免重载和偏特化的混淆
 *
 * 现代cpp的generic programming
 * auto
 * decltype
 * forwarding reference，转发引用，有一套特殊的推导规则
 * 引用折叠，四种引用折叠，只要出现左值引用，那么结果就是左值引用
 * 完美转发
 *
 * Q：为什么要支持同时传入左值和右值？
 * A：
 * 引用折叠不是为了创造一种新的引用类型，而是为了让模板推导产生的引用的引用有一个合法的、符合直觉的结果
 * const左值可以绑定一个临时右值
 * 引用折叠是为了避免丢失左值和右值的信息，决定拷贝语义
 *
 *
 *
 *
 * 什么是concept lifting？
 *
 *
 *
 *
 * RAII的思想：利用局部对象的生命周期来管理资源释放
 * 构造函数中获取资源，打开文件，加锁，分配内存等
 * 析构函数中释放资源，打开文件，解锁，释放内存等
 * @return
 */
int main(){
//    int a = 10, b = 20;
//    cout << max_(a, b)<< endl;
    test_remain_value_category();
    return 0;
}