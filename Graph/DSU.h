#ifndef DSU_H
#define DSU_H
#include<vector>
class DSU
{
public:
    DSU(size_t size){
        parent.reserve(size);
        rank.reserve(size);
    }
    void make_set (int v) {
        parent[v] = v;
        rank[v] = 0;
    }

    int find_set (int v) {
        if (v == parent[v])
            return v;
        return parent[v] = find_set (parent[v]);
    }

    void union_sets (int a, int b) {
        a = find_set (a);
        b = find_set (b);
        if (a != b) {
            if (rank[a] < rank[b])
                std::swap (a, b);
            parent[b] = a;
            if (rank[a] == rank[b])
                ++rank[a];
        }
    }
private:
   std::vector<int> parent;
   std::vector<int> rank;
}
;

#endif // DSU_H
