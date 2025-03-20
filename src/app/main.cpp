# include <bits/stdc++.h>
# include "./graph/graph_utils.h"
# include <fstream>
using namespace std;

int main() {

    ifstream infile("input.txt");
    ofstream outfile("output.txt");

    if(!infile || !outfile){
        cerr << "Error opening file!" << '\n';
        return 1;
    }
    outfile << "Welcome to our BTP Project!" << '\n' << '\n';
    int n,e;
    infile >> n >> e;
    vector<vector<pair<int,int>>> adj(n);
    map<pair<int,int>,int> edge_wt_map;
    for(int i=0; i<e; i++){
        int u,v,ewt;
        infile >> u >> v >> ewt;
        adj[u].push_back({v,ewt});
        adj[v].push_back({u,ewt});
        edge_wt_map[{u,v}] = ewt;
        edge_wt_map[{v,u}] = ewt;
    }

    int src,dest;
    infile >> src >> dest;

    vector<vector<int>> parent1(n);

    BFS(src, n, parent1, adj);

    set<vector<int>> paths;
    vector<int> path;

    FIND_PATHS(dest, path, parent1, paths);

    map<int,set<int>> d_star;

    outfile << "All shortest-paths from src " << src << " to dest " << dest << '\n';
    for(auto p: paths){
        reverse(p.begin(), p.end());

        for(int i=0; i<p.size(); i++){
            if(i<p.size()-1) d_star[p[i]].insert(p[i+1]);
            if(i==p.size()-1) outfile << p[i];
            else outfile << p[i] << " -> ";
        }
        outfile << '\n';
    }

    outfile << "\nDerived D* Graph" << '\n';
    for(auto it:d_star){
        int node = it.first;
        outfile << "----------- \n";
        outfile << "Node: " << node << "\n";
        for(auto j: it.second){
            outfile << node << " => " << "{ " << j << " , " << edge_wt_map[{node,j}] << " }"  << '\n';
        }
        outfile << "----------- \n";
    }

    vector<int> parent2(n);
    SPT_DIJKSTRA(dest, n, parent2, adj);
    outfile << "Shortest-path Tree: " << '\n';
    for(int i=0; i<n; i++){
        outfile << "Node: " << i << ", " << "Parent: " << parent2[i] << '\n';
    }
    outfile << "----------- \n";

    infile.close();
    outfile.close();

    return 0;
}