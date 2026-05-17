//Jonathan Attanasio
//This program takes an input and determines if there is an edge between the two nodes in the graph.
//It checks this by DFS.

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <sstream>

using namespace std;

// Function to perform Depth Search
bool dfs(const unordered_map<string, vector<string>>& graph, const string& src, const string& dest, unordered_set<string>& visited) {
    if (src == dest) {
        return true;
    }

    // Check if the source node exists in the graph
    if (graph.find(src) == graph.end()) {
        return false;  // Source does not exist in the graph
    }

    visited.insert(src);

    // Explore each connected neighbor of the node currently being assessed
    for (const auto& neighbor : graph.at(src)) {
        if (visited.find(neighbor) == visited.end()) {
            if (dfs(graph, neighbor, dest, visited)) {
                return true;
            }
        }
    }

    return false;
}

// Function to check for a path between the two nodes that were given by the input file
bool check_path(const unordered_map<string, vector<string>>& graph, const string& src, const string& dest) {
    unordered_set<string> visited;  // Set to keep track of visited nodes
    return dfs(graph, src, dest, visited);
}

int main() {
    string line;
    int graph_count = 1;

    while (getline(cin, line)) {
        // Read number of edges
        int n_edges = stoi(line);
        unordered_map<string, vector<string>> graph;

        // Read edges
        for (int i = 0; i < n_edges; ++i) {
            getline(cin, line);
            stringstream ss(line);
            string src, dst;
            ss >> src >> dst;
            graph[src].push_back(dst);  // Add edge to the graph
        }

        // Read number of paths
        int n_paths;
        getline(cin, line);
        n_paths = stoi(line);

        // Check paths
        for (int i = 0; i < n_paths; ++i) {
            getline(cin, line);
            stringstream ss(line);
            string src, dst;
            ss >> src >> dst;
            if (check_path(graph, src, dst)) {
                cout << "In Graph " << graph_count << " there is a path from " << src << " to " << dst << endl;
            } else {
                cout << "In Graph " << graph_count << " there is no path from " << src << " to " << dst << endl;
            }
        }

        graph_count++;
        cout << endl; // Print an empty line between graphs
    }

    return 0;
}
