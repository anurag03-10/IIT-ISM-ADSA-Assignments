#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <unordered_map>
#include <algorithm>
using namespace std;

struct Edge { int to; int rev; int cap; };

void addEdge(vector<vector<Edge>>& g, int u, int v, int c) {
    g[u].push_back({v, (int)g[v].size(), c});
    g[v].push_back({u, (int)g[u].size()-1, 0});
}

// BFS to find augmenting path; returns flow added and records parent edges
int bfs(int s, int t, vector<vector<Edge>>& g, vector<pair<int,int>>& parent) {
    int n = g.size();
    vector<int> cap(n, 0);
    queue<int> q;
    q.push(s); cap[s] = INT_MAX;
    parent.assign(n, {-1,-1});
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int i = 0; i < (int)g[u].size(); ++i) {
            Edge &e = g[u][i];
            if (cap[e.to] == 0 && e.cap > 0) {
                parent[e.to] = {u, i};
                cap[e.to] = min(cap[u], e.cap);
                if (e.to == t) return cap[t];
                q.push(e.to);
            }
        }
    }
    return 0;
}

int maxFlow(int s, int t, vector<vector<Edge>>& g, vector<vector<pair<int,int>>>& pathsUsed, vector<int>& flowsAdded) {
    int flow = 0;
    int n = g.size();
    vector<pair<int,int>> parent(n);
    while (true) {
        int added = bfs(s, t, g, parent);
        if (added == 0) break;
        // reconstruct path
        int v = t; vector<int> pathNodes;
        while (v != s) {
            auto pr = parent[v];
            pathNodes.push_back(v);
            v = pr.first;
        }
        pathNodes.push_back(s);
        reverse(pathNodes.begin(), pathNodes.end());

        // update residual graph
        v = t;
        while (v != s) {
            auto pr = parent[v];
            int u = pr.first; int ei = pr.second;
            g[u][ei].cap -= added;
            int rev = g[u][ei].rev;
            int to = g[u][ei].to;
            // find reverse edge index
            int revIdx = g[to][g[u][ei].rev].rev; // not used
            g[to][g[u][ei].rev].cap += added;
            v = u;
        }

        pathsUsed.push_back({});
        for (int node : pathNodes) pathsUsed.back().push_back({node, -1});
        flowsAdded.push_back(added);
        flow += added;
    }
    return flow;
}

int main() {
    // map nodes S,A,B,C,D,T to indices
    unordered_map<string,int> idx = {{"S",0},{"A",1},{"B",2},{"C",3},{"D",4},{"T",5}};
    int N = 6;
    vector<vector<Edge>> graph(N);

    // initial capacities as per problem
    addEdge(graph, idx["S"], idx["A"], 16);
    addEdge(graph, idx["S"], idx["C"], 13);
    addEdge(graph, idx["A"], idx["B"], 12);
    addEdge(graph, idx["B"], idx["C"], 9);
    addEdge(graph, idx["C"], idx["A"], 4);
    addEdge(graph, idx["C"], idx["D"], 14);
    addEdge(graph, idx["D"], idx["B"], 7);
    addEdge(graph, idx["B"], idx["T"], 20);
    addEdge(graph, idx["D"], idx["T"], 4);

    // make a copy to run the second experiment later
    auto graph2 = graph;

    vector<vector<pair<int,int>>> pathsUsed;
    vector<int> flowsAdded;
    int maxflow = maxFlow(idx["S"], idx["T"], graph, pathsUsed, flowsAdded);

    cout << "Results for original capacities:\n";
    for (size_t i = 0; i < pathsUsed.size(); ++i) {
        cout << "Route " << i+1 << ": ";
        for (size_t j = 0; j < pathsUsed[i].size(); ++j) {
            string name;
            for (auto &p : idx) if (p.second == pathsUsed[i][j].first) name = p.first;
            cout << name;
            if (j+1 < pathsUsed[i].size()) cout << " -> ";
        }
        cout << "\nFlow added: " << flowsAdded[i] << "\n\n";
    }

    // show final flows on edges (original capacities - residual capacity of forward edges)
    cout << "Final edge flows:\n";
    vector<tuple<string,string,int>> edges = {{"S","A",16},{"S","C",13},{"A","B",12},{"B","C",9},{"C","A",4},{"C","D",14},{"D","B",7},{"B","T",20},{"D","T",4}};
    for (auto &e : edges) {
        string u,v; int cap; tie(u,v,cap) = e;
        int uidx = idx[u];
        // find forward edge cap (residual) and compute flow = original - residual
        int residual = 0;
        for (auto &ed : graph[uidx]) {
            if (ed.to == idx[v]) { residual = ed.cap; break; }
        }
        int flowOnEdge = cap - residual;
        cout << u << " -> " << v << ": " << flowOnEdge << " / " << cap << "\n";
    }

    cout << "\nFinal Maximum Flow = " << maxflow << "\n\n";

    // Additional task: increase D->T capacity from 4 to 10 and rerun
    // Instead reconstruct graph2 with modified capacity (D->T = 10)
    vector<vector<Edge>> graph3(N);
    addEdge(graph3, idx["S"], idx["A"], 16);
    addEdge(graph3, idx["S"], idx["C"], 13);
    addEdge(graph3, idx["A"], idx["B"], 12);
    addEdge(graph3, idx["B"], idx["C"], 9);
    addEdge(graph3, idx["C"], idx["A"], 4);
    addEdge(graph3, idx["C"], idx["D"], 14);
    addEdge(graph3, idx["D"], idx["B"], 7);
    addEdge(graph3, idx["B"], idx["T"], 20);
    addEdge(graph3, idx["D"], idx["T"], 10); // increased

    vector<vector<pair<int,int>>> pathsUsed2;
    vector<int> flowsAdded2;
    int maxflow2 = maxFlow(idx["S"], idx["T"], graph3, pathsUsed2, flowsAdded2);

    cout << "Results after increasing D->T capacity to 10:\n";
    for (size_t i = 0; i < pathsUsed2.size(); ++i) {
        cout << "Route " << i+1 << ": ";
        for (size_t j = 0; j < pathsUsed2[i].size(); ++j) {
            string name;
            for (auto &p : idx) if (p.second == pathsUsed2[i][j].first) name = p.first;
            cout << name;
            if (j+1 < pathsUsed2[i].size()) cout << " -> ";
        }
        cout << "\nFlow added: " << flowsAdded2[i] << "\n\n";
    }
    cout << "Final Maximum Flow (after increase) = " << maxflow2 << "\n\n";

    cout << "Bottleneck analysis: Compare the two max flows to see effect of increasing D->T.\n";
    return 0;
}
