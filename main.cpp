#include "include/vector.h"
#include <vector>
#include <iostream>
#include <chrono>

int main()
{
    int sz = 100000000;
    auto start = std::chrono::system_clock::now();

    std::vector<int> v1;
    for (int i = 1; i <= sz; ++i)
        v1.push_back(i);

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "std::vector užtruko " << elapsed_seconds.count() << std::endl;

    start = std::chrono::system_clock::now();

    Vector<int> v2;
    for (int i = 1; i <= sz; ++i)
        v2.push_back(i);

    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    std::cout << " Vector užtruko " << elapsed_seconds.count() << std::endl;

    return 0;
}