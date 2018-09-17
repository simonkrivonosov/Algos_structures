#ifndef MINCOSTMAXFLOW_H
#define MINCOSTNetWork_H

#include <stdio.h>
#include <vector>
#include <iostream>
#include <assert.h>
#include <climits>

class NetWork {
public:
    typedef std::vector<std::vector<std::pair<double, double> > >
    Edges;

    NetWork(int n);

    void AddEdge(int from, int to, double capacity, double cost);

    void Get_Shortest_Path(Edges &flow,
                 std::vector<double>& dist,
                 std::vector<int>& path,
                 double& allowed_flow, int s, int t);

    std::pair<double, double> Flow(int s, int t, double max_flow);

    friend NetWork& operator>>(std::istream& stream, NetWork& net);
private:
    Edges _network;
    std::vector<std::vector<int>> _indexes;
};

#endif // MINCOSTMAXFLOW_H
