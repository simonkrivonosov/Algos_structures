#include <iostream>
#include "MinCostMaxFlow.h"
using namespace std;

int main()
{
    int n;
                  std::cin >> n;
                  NetWork m(n);

                  int t, s;
                  std::cin >> t;
                  std::cin >> s;

                  std::cin >> m;
                  std::pair<double, double> ans = m.Flow(t, s, INT_MAX);
                  std::cout << ans.first << ' ' << ans.second;
                  return 0;
}
//6
//1 5
//7
//0 1 2 2
//1 3 5 3
//1 4 1 1
//1 2 10 4
//2 4 1 5
//3 4 2 2
//4 5 1 3

