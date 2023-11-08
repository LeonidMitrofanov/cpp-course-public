#include "bin_search.hpp"

#include "iostream"
#include "stdlib.h"
#include <vector>

int main()
{
    // nullptr - не должны падать, вернуть nullptr
    // ТУТ ЗАГЛУШКА, ЧТОБЫ КОД КОМПИЛИРОВАЛСЯ
    
    std::vector<int> v{0,1,2,3,4,5,6,7, 999}; // 999 - значение вне массива
    std::vector<int> v1;

    std::cout << *LowerBound(v.data(), v.data()+v.size()-1 ,1) << std::endl; 
    return 0;
}
