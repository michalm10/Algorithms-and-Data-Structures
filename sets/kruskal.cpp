#include <iostream>
#include <algorithm>
#include "kruskal.h"

struct {
    bool operator()(Edge a, Edge b) const { return a.weight < b.weight; }
} cmp;

void Kruskal(int vertexCount, std::vector<Edge> Edges) {
    Node *sets[vertexCount];
    for (int i = 0; i < vertexCount; i++)
        sets[i] = makeSet(i);
    sort(Edges.begin(), Edges.end(), cmp);
    for (auto x : Edges) {
        auto ru = findSet(sets[x.u]);
        auto rv = findSet(sets[x.v]);
        if (ru != rv) {
            x.print();
            Union(ru, rv);
        }
    }
}
