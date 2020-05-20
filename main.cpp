#include "include/vector.h"
#include <vector>
#include <iostream>
#include "include/john_vector.h"

int main()
{
   // rice::vector<int> vstd{1,2,3, 4, 5 ,6,7,8,9};
    //Vector<int> v{1,2,3, 4, 5 ,6,7,8,9};
    Vector<int> a (15, 8);
    std::cout << "size " << a.size() << std::endl;
    a.resize(15, 1);
    std::cout << " " << a.size() << std::endl;
    for (auto &element : a)
        std::cout << element << " "; 
   
    //std::cout << " size " << v.capacity() << std::endl;
   /*  v.clear();
    std::cout << v.size();
    //std::cout << " size " << v.capacity() << std::endl;

    for (auto &element : v)
        std::cout << element << " ";   */
    
    return 0;
}