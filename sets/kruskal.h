#pragma once
#include <vector>

class Node {
public:
    double key;
    int rank = 0;
    Node *p = this;
    Node(double key) {
        this->key = key;
    }
    Node(double key, Node& parent) {
        key = key;
        p = &parent;
    }
    ~Node() {std::cout << "co\n";}
};

struct Edge {
    int u, v, weight;
    void print() {
        std::cout << u + 1 << "-" << v + 1 << ": " << weight << std::endl;
    }
};

Node* makeSet(double k);
Node* findSet(Node *x);
void Union(Node *x, Node *y);
void Kruskal(int vertexCount, std::vector<Edge> Edges);
