#include "include/vector.h"
#include <vector>
#include <iostream>

int main()
{
    std::vector<int> vstd(5,2);
    Vector<int> v{1,2,3, 4, 5 };
   // std::cout << v.capacity() << std::endl;

    v.erase(v.begin());

    for (auto &element : v)
        std::cout << element << " ";

   // std::cout << v.capacity();
    

    return 0;
}