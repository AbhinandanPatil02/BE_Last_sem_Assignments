#include <bits/stdc++.h>
#include <omp.h>
#include <chrono>
using namespace std;

class Graph {

private:

    unordered_map<int, vector<int>> adj;
    unordered_map<int, bool> visited;

public:
    // Add edge (undirected)
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Reset visited
    void resetVisited() {
        for (auto &node : adj) {
            visited[node.first] = false;
        }
    }

    // Print graph
    void printGraph() {
        cout << "\nAdjacency List:\n";
        for (auto &node : adj) {
            cout << node.first << " -> ";
            for (auto &nbr : node.second) {
                cout << nbr << " ";
            }
            cout << endl;
        }
    }

    // ---------------- DFS ----------------
    void dfs(int start) {
        stack<int> st;
        st.push(start);
        visited[start] = true;

        while (!st.empty()) {
            int curr = st.top();
            st.pop();

            cout << curr << " ";

            for (auto nbr : adj[curr]) {
                if (!visited[nbr]) {
                    visited[nbr] = true;
                    st.push(nbr);
                }
            }
        }
    }

    // ---------------- PARALLEL DFS ----------------
    void parallel_dfs(int start) {
        stack<int> st;
        st.push(start);
        visited[start] = true;

        while (!st.empty()) {
            int curr;

            #pragma omp critical
            {
                if (!st.empty()) {
                    curr = st.top();
                    st.pop();
                    cout << curr << " ";
                }
            }

            #pragma omp parallel for
            for (int i = 0; i < adj[curr].size(); i++) {
                int nbr = adj[curr][i];

                if (!visited[nbr]) {
                    #pragma omp critical
                    {
                        if (!visited[nbr]) {
                            visited[nbr] = true;
                            st.push(nbr);
                        }
                    }
                }
            }
        }
    }

    // ---------------- BFS ----------------
    void bfs(int start) {
        queue<int> q;
        q.push(start);
        visited[start] = true;

        while (!q.empty()) {
            int curr = q.front();
            q.pop();

            cout << curr << " ";

            for (auto nbr : adj[curr]) {
                if (!visited[nbr]) {
                    visited[nbr] = true;
                    q.push(nbr);
                }
            }
        }
    }

    // ---------------- PARALLEL BFS ----------------
    void parallel_bfs(int start) {
        vector<int> frontier;
        frontier.push_back(start);
        visited[start] = true;

        while (!frontier.empty()) {
            vector<int> next_frontier;

            #pragma omp parallel
            {
                vector<int> local_next;

                #pragma omp for nowait
                for (int i = 0; i < frontier.size(); i++) {
                    int curr = frontier[i];
                    cout << curr << " ";
                    for (auto nbr : adj[curr]) {
                        if (!visited[nbr]) {
                            #pragma omp critical
                            {
                                if (!visited[nbr]) {
                                    visited[nbr] = true;
                                    local_next.push_back(nbr);
                                }
                            }
                        }
                    }
                }

                #pragma omp critical
                next_frontier.insert(next_frontier.end(),
                                     local_next.begin(),
                                     local_next.end());
            }

            frontier = next_frontier;
        }
    }
};

int main() {
    Graph g;

    int e;
    cout << "Enter number of edges: ";
    cin >> e;

    cout << "Enter edges (u v):\n";
    for (int i = 0; i < e; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    g.printGraph();

    int choice, start;

    do {
        cout << "\n1. DFS\n2. Parallel DFS\n3. BFS\n4. Parallel BFS\n5. Exit\n";
        cin >> choice;

        if (choice >= 1 && choice <= 4) {
            cout << "Enter start node: ";
            cin >> start;
            g.resetVisited();
            auto t1 = chrono::high_resolution_clock::now();

            if (choice == 1) g.dfs(start);
            else if (choice == 2) g.parallel_dfs(start);
            else if (choice == 3) g.bfs(start);
            else if (choice == 4) g.parallel_bfs(start);

            auto t2 = chrono::high_resolution_clock::now();

            cout << "\nTime: "
                 << chrono::duration_cast<chrono::microseconds>(t2 - t1).count()
                 << " microseconds\n";
        }

    } while (choice != 5);
}