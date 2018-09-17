#include <iostream>
#include "KMP.h"
using namespace std;

int main()
{
    string s1;
    string s2;
    std::cout<<"Vvedite podstroku :"<<std::endl;
    std::cin >> s1;
    std::cout<<std::endl;
    std::cout<<"Vvedite stroku, gde budem iskat:"<<std::endl;
    std::cin >> s2;
    std::cout<<std::endl;
    std::vector<size_t> res = KMP(s1,s2);
    for (size_t i = 0; i < res.size(); ++i)
    {
        std::cout<<res[i]<<std::endl;
    }
}

