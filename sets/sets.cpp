#include <iostream>
#include "kruskal.h"

using namespace std;

Node* makeSet(double k) {
    return new Node(k);
}

Node* findSet(Node *x) {
    if (x != x->p)
        x->p = findSet(x->p);
    return x->p;
}

void Union(Node *x, Node *y) {
    if (x->rank < y->rank)
        x->p = y;
    else {
        y->p = x;
        if (x->rank == y->rank)
            x->rank++;
    }
}

int main () {
    Node *z0 = makeSet(0);
    Node *z1 = makeSet(1);
    Node *z2 = makeSet(2);
    Node *z3 = makeSet(3);
    Node *z4 = makeSet(4);
    Node *z5 = makeSet(5);
    Node *z6 = makeSet(6);
    Node *z7 = makeSet(7);
    Node *z8 = makeSet(8);
    Node *z9 = makeSet(9);

    Union(findSet(z0), findSet(z1));
    Union(findSet(z2), findSet(z3));
    Union(findSet(z1), findSet(z2));
    Union(findSet(z5), findSet(z6));
    Union(findSet(z7), findSet(z8));
    Union(findSet(z3), findSet(z5));
    Union(findSet(z0), findSet(z7));

    Node *sets[] = {z0, z1, z2, z3, z4, z5, z6, z7, z8, z9};
    for (unsigned int i = 0; i < sizeof(sets) / sizeof(sets[0]); i++) {
        cout << "z" << i << ": ";
        Node *iter = sets[i];
        cout << iter->key << " ";
        while (iter->p != iter) {
            cout << iter->p->key << " ";
            iter = iter->p;
        }
        cout << endl;
    }

    vector<Edge> Edges {
        {0, 1, 4},
        {1, 2, 10},
        {2, 3, 9},
        {3, 4, 7},
        {4, 5, 10},
        {5, 6, 2},
        {6, 7, 1},
        {7, 0, 9},
        {1, 7, 11},
        {3, 5, 8},
        {2, 5, 4},
        {2, 8, 2},
        {8, 7, 7},
        {8, 6, 6}
    };
    cout << endl;
    Kruskal(9, Edges);
    return 0;
}
