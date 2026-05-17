// Challenge 6: Minimum Spanning Tree - Jonathan Attanasio
// This program uses Prim's algorithm to find the MST in a graph. The graph is given by the input file.
// This is then iterated over to find the smallest total weight to visit each node.

#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

// Function to print the MST result and formatted appropriately for output
void printMST(int total_weight, vector<pair<int, int>>& mst_edges) {
    cout << total_weight << endl;
    sort(mst_edges.begin(), mst_edges.end(), [](pair<int, int> a, pair<int, int> b) {
        if (a.first == b.first) return a.second < b.second;
        return a.first < b.first;
    });
    for (auto edge : mst_edges) {
        cout << char('A' + edge.first) << char('A' + edge.second) << endl;
    }
}

int primMST(int vertices, const vector<vector<int>>& matrix) {
    vector<bool> in_mst(vertices, false);  // Track vertices included in MST
    vector<int> min_edge(vertices, INT_MAX);  // Store the minimum edge weight to each vertex
    vector<int> parent(vertices, -1);  // Store the parent vertex for each vertex
    min_edge[0] = 0;  // Start from vertex A (index 0)

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;  // Min-heap
    pq.push({0, 0});  // {edge_weight, vertex}

    vector<pair<int, int>> mst_edges;  // To store the edges in the MST
    int total_weight = 0;

    while (!pq.empty()) {
        int u = pq.top().second;
        int weight = pq.top().first;
        pq.pop();

        if (in_mst[u]) continue;  // Skip if already in MST, similar to Dijkstra's where there is a visited component to the code

        in_mst[u] = true;
        total_weight += weight;

        if (parent[u] != -1) {
            mst_edges.push_back({min(parent[u], u), max(parent[u], u)});
        }

        // Explore neighbors and push them into the priority queue
        for (int v = 0; v < vertices; ++v) {
            if (matrix[u][v] != -1 && !in_mst[v] && matrix[u][v] < min_edge[v]) {
                min_edge[v] = matrix[u][v];
                parent[v] = u;
                pq.push({min_edge[v], v});
            }
        }
    }

    printMST(total_weight, mst_edges);
    return total_weight;
}

int main() {
    int vertices;
    bool first_case = true;

    while (cin >> vertices) {
        if (!first_case) {
            cout << endl;  // Blank line between different graphs via the requested output
        }
        first_case = false;

        // Read the distance matrix
        vector<vector<int>> matrix(vertices, vector<int>(vertices));
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                cin >> matrix[i][j];
            }
        }

        // Run Prim's algorithm to compute the Minimum Spanning Tree
        primMST(vertices, matrix);
    }

    return 0;
}

