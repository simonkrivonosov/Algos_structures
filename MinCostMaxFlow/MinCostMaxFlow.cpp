
#include "MinCostMaxFlow.h"

NetWork::NetWork(int n): _network(n,std::vector<std::pair<double, double> >(n,std::pair<double, double> ())),
    _indexes(n) {

}

void NetWork::AddEdge(int from, int to, double capacity, double cost) {
    _indexes[from].push_back(to);
    _indexes[to].push_back(from);
    _network[from][to] = {capacity, cost};
    _network[to][from] = {0, -cost};
}

void NetWork::Get_Shortest_Path(Edges& flow,
                      std::vector<double>& dist,
                      std::vector<int>& path,
                      double& allowed_flow, int s, int t) {
    dist = std::vector<double> (_network.size(), INT_MAX);
    std::vector<int> pred(_network.size(), INT_MAX);
    dist[s] = 0;

    for (size_t i = 0; i <_network.size(); ++i) {
        for (size_t u = 0; u < _network.size(); ++u) {
            for (size_t v = 0; v < _indexes[u].size(); ++v) {
                if (
                    dist[u] != INT_MAX &&
                    flow[u][_indexes[u][i]].second < _network[u][_indexes[u][i]].first &&
                    dist[_indexes[u][i]] > dist[u] + _network[u][_indexes[u][i]].second) {

                    dist[_indexes[u][i]] = dist[u] + _network[u][_indexes[u][i]].second;
                    pred[_indexes[u][i]] = u;

                }
            }
        }
    }

    path.clear();

    if (dist[t] >= INT_MAX) {
        return;
    }

    allowed_flow = INT_MAX;
    for (int i = t; i != s; i = pred[i]) {
        path.push_back(i);
        if (allowed_flow > flow[pred[i]][i].first-flow[pred[i]][i].second) {
            allowed_flow = flow[pred[i]][i].first-flow[pred[i]][i].second;
        }
    }
    path.push_back(s);
}

std::pair<double, double> NetWork::Flow(int s, int t, double max_flow) {
    Edges flow(_network);
    for (size_t i = 0; i < _network.size(); ++i) {
        for (size_t j = 0; j < _network.size(); ++j) {
            flow[i][j].second = 0;
        }
    }
    std::vector<int> path;
    std::vector<double> dist;
    double curflow = 0;
    double flow_cost = 0;
    double allowed_flow = 0;

    while (curflow < max_flow) {
        Get_Shortest_Path(flow, dist, path, allowed_flow, s, t);
        if (dist[t] == INT_MAX) {
            break;
        }
        for (size_t i = 0; i < path.size()-1; ++i) {

            int from = path[path.size()-i-1];
            int to = path[path.size()-i-2];

            flow[from][to].second += allowed_flow;
            flow[to][from].second -= allowed_flow;

            flow_cost += _network[from][to].second * allowed_flow;
        }
        curflow += allowed_flow;
    }

    return {curflow, flow_cost};
}

NetWork& operator>>(std::istream& stream, NetWork& net) {
    int n,from,to;
    double cost,capacity;
    stream >> n;
    for (int i = 0; i < n; ++i) {
        stream >> from;
        stream >> to;
        stream >> cost;
        stream >> capacity;
        net.AddEdge(from, to, capacity, cost);
    }
    return net;
}
