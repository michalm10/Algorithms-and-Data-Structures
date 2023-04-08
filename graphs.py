from cgi import test
from queue import Queue

def loadFromFile(filename):
    with open(filename) as f:
        array = [[int(x) for x in line.split()] for line in f]
    return array, len(array) == len(array[0]) + 1

class Vertex:
    nr = 0
    visited = False
    p = None
    Adj = None
    def __init__(self, nr):
        self.nr = nr
    def __repr__(self):
        return str(self.nr) + str([self.Adj[i].nr for i in range(len(self.Adj))])

def createVertexList(graph):
    if len(graph) == 0:
        return
    Vertexes = [Vertex(x) for x in graph[0]]
    for i, x in enumerate(graph[1:]):
        Vertexes[i].Adj = [Vertexes[j] for j, y in enumerate(x) if y > 0]
    return Vertexes

def depthFirstSearch(Vertexes):
    Visited = []
    Edges = []
    dfs(Vertexes, Vertexes[0], Visited, Edges)
    print("Przeszukiwanie wgłąb:")
    for x in Visited:
        print(x.nr, end=' ')
    print()
    for x in Edges:
        print(x[0].nr, '-', x[1].nr)

def dfs(Vertexes, u, Visited, Edges):
    Visited.append(u)
    u.visited = True
    for v in u.Adj:
        if v.visited == False:
            Edges.append([u, v])
            v.p = u
            dfs(Vertexes, v, Visited, Edges)

def breadthFirstSearch(Vertexes):
    Visited = []
    Edges = []
    Q = Queue()
    bfs(Vertexes[0], Q, Visited, Edges)
    print("Przeszukiwanie wszerz:")
    for x in Visited:
        print(x.nr, end=' ')
    print()
    for x in Edges:
        print(x[0].nr, '-', x[1].nr)

def bfs(s, Q, Visited, Edges):
    s.visited = True
    Q.put(s)
    while not Q.empty():
        u = Q.get()
        Visited.append(u)
        if not u.p == None:
            Edges.append([u.p, u])
        for v in u.Adj:
            if v.visited == False:
                v.visited = True
                v.p = u
                Q.put(v)

def main():
    testGraph, isSquare = loadFromFile('testgraph.txt')
    if not isSquare:
        return
    Vertexes = createVertexList(testGraph)
    VertexesCopy = createVertexList(testGraph)
    depthFirstSearch(Vertexes)
    breadthFirstSearch(VertexesCopy)
    print(Vertexes)

if __name__ == "__main__":
    main()