#include <iostream>

#include "curry.h"


int addThree(int a, int b, int c)
{
    return a + b + c;
}

int addTwo(int a, int b, int& out)
{
    static int numCalled = 0;
    out = ++numCalled;

    return a + b;
}

int main() {
    CurriedFunction<int(int, int, int)> func;

    std::cout << "function pointer:\n";
    func = &addThree;
    std::cout << func(1)(2)(3) << std::endl;
    std::cout << func(1)(2, 3) << std::endl;
    std::cout << func(1, 2)(3) << std::endl;
    std::cout << func(1, 2, 3) << std::endl;
    std::cout << func()(1)()(2)()()()(3) << std::endl;
    std::cout << std::endl;

    std::cout << "lambda:\n";
    func = [](int a, int b, int c) { return a + b + c; };
    std::cout << func(1)(2)(3) << std::endl;
    std::cout << func(1)(2, 3) << std::endl;
    std::cout << func(1, 2)(3) << std::endl;
    std::cout << func(1, 2, 3) << std::endl;
    std::cout << func()(1)()(2)()()()(3) << std::endl;
    std::cout << std::endl;

    std::cout << "captures:\n";
    int count1 = 0;
    func = [&count1](int a, int b, int c) { ++count1; return a + b + c; };
    std::cout << func(1)(2)(3) << std::endl;
    std::cout << func(1)(2, 3) << std::endl;
    std::cout << func(1, 2)(3) << std::endl;
    std::cout << func(1, 2, 3) << std::endl;
    std::cout << func()(1)()(2)()()()(3) << std::endl;
    std::cout << "count: " << count1 << std::endl;
    std::cout << std::endl;

    std::cout << "forwarding:\n";
    int count2 = 0;
    CurriedFunction<int(int, int, int&)> fptr2 = &addTwo;
    std::cout << fptr2(1)(2)(count2) << ", count: " << count2 << std::endl;
    std::cout << fptr2(1)(2, count2) << ", count: " << count2 << std::endl;
    std::cout << fptr2(1, 2)(count2) << ", count: " << count2 << std::endl;
    std::cout << fptr2(1, 2, count2) << ", count: " << count2 << std::endl;
    std::cout << fptr2()(1)()(2)()()()(count2) << ", count: " << count2 << std::endl;
    std::cout << std::endl;

    return 0;
}
