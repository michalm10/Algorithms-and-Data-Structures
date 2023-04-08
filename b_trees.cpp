#include <iostream>
#include <fstream>
#include <random>

using namespace std;

#define T 3

class Node {
public:
    int keys[2 * T - 1];
    int keysCount = 0;
    int connections[2 * T];
    bool isLeaf = false;
    int TPos;
    static int fPos;
    Node (): TPos{0} {}
    Node (int a) {
        TPos = fPos;
        fPos++;
    }
    void save(fstream& file) {
        file.seekg(TPos * sizeof(Node), file.beg);
        file.write((char*) this, sizeof(Node));
    }
};
int Node::fPos = 0;

class B_tree {
public:
    int height = 1;
    Node root = Node(0);
    fstream file;
    B_tree(string fileName, int key) {
        file.open(fileName, ios::out|ios::in|ios::binary);
        if (!file.is_open()) {
            cout << "File opening failed" << endl;
            exit(1);
        }
        root.keys[0] = key;
        root.isLeaf = true;
        root.keysCount++;
        root.save(file);
    }
    Node read(int fPos);
    Node* find(int key, int& pos);
    void insert(int key);
    void display();
private:
    void print(int fPos, int height);
    Node* f(Node& n, int key, int& pos);
    void split(Node& p, Node& c, int k);
    void insertNonfull(Node& n, int key);
};

Node B_tree::read(int fPos) {
    Node n;
    file.seekg(fPos * sizeof(Node), file.beg);
    file.read((char*) &n, sizeof(Node));
    return n;
}

void B_tree::split(Node& p, Node& c, int k) {
    Node n(0);
    n.isLeaf = c.isLeaf;
    n.keysCount = T - 1;
    for (int i = 0; i < n.keysCount; i++)
        n.keys[i] = c.keys[i + T];
    if (!c.isLeaf)
        for (int i = 0; i < T; i++)
            n.connections[i] = c.connections[i + T];
    c.keysCount = T - 1;
    for (int i = p.keysCount + 1; i > k + 1; i--)
        p.connections[i] = p.connections[i - 1];
    p.connections[k + 1] = n.TPos;
    for (int i = p.keysCount; i > k; i--)
        p.keys[i] = p.keys[i - 1];
    p.keys[k] = c.keys[T - 1];
    p.keysCount++;
    p.save(file);
    c.save(file);
    n.save(file);
}

void B_tree::insertNonfull(Node& n, int key) {
    int i = n.keysCount;
    if (n.isLeaf) {
        while (i > 0 && key < n.keys[i - 1]) {
            n.keys[i] = n.keys[i - 1];
            i--;
        }
        n.keys[i] = key;
        n.keysCount++;
        n.save(file);
    }
    else {
        while (i > 0 && key < n.keys[i - 1]) i--;
        Node l = Node();
        l = read(n.connections[i]);
        if (l.keysCount == 2 * T - 1) {
            split(n, l, i);
            if (key > n.keys[i]) i++;
        }
        l = read(n.connections[i]);
        insertNonfull(l, key);
    }
}

void B_tree::insert(int key) {
    if (root.keysCount == 2 * T - 1) {
        Node s = Node(0);
        int pos = s.TPos;
        s = root;
        s.TPos = pos;
        s.isLeaf = false;
        s.keysCount = 0;
        s.connections[0] = root.TPos;
        s.save(file);
        split(s, root, 0);
        root = s;
        insertNonfull(s, key);
    }
    else {
        insertNonfull(root, key);
    }
}

Node* B_tree::find(int key, int& pos) {
    return f(root, key, pos);
}

Node* B_tree::f(Node& n, int key, int& pos) {
    int i = 0;
    while (i < n.keysCount && key > n.keys[i]) i++;
    if (i < n.keysCount && key == n.keys[i]) {
        pos = i;
        return &n;
    }
    if (n.isLeaf) {
        pos = -1;
        return &n;
    }
    else {
        Node l = read(n.connections[i]);
        return f(l, key, pos);
    }
}

void B_tree::display() {
    cout << "----------------------------------------------" << endl;
    print(root.TPos, 0);
    cout << "----------------------------------------------" << endl;
}

void B_tree::print(int fPos, int height) {
    Node n = Node();
    n = read(fPos);
    if (n.isLeaf) {
        for (int i = 0; i < height; i++) cout << '\t';
        for (int i = 0; i < n.keysCount - 1; i++) cout << n.keys[i] << "; ";
        cout << n.keys[n.keysCount - 1] << endl;
    }
    else {
        print(n.connections[n.keysCount], height + 1);
        for (int i = n.keysCount - 1; i >= 0; i--) {
            for (int j = 0; j < height; j++) cout << '\t';
            cout << n.keys[i] << endl;
            print(n.connections[i], height + 1);
        }
    }
}

int main () {
    random_device d;
    mt19937 rng(d());
    uniform_int_distribution<mt19937::result_type> randomKey(1, 100);
    B_tree testTree("test.bin", 15);
    for (int i = 0; i < 100; i++) {
        testTree.insert(randomKey(rng));
    }
    int pos;
    testTree.display();
    Node n = *testTree.find(97, pos);
    if (pos >= 0) cout << n.keys[pos] << endl;
    n = *testTree.find(35, pos);
    if (pos >= 0) cout << n.keys[pos] << endl;
    return 0;
}
