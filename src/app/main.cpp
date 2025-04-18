# include <bits/stdc++.h>
# include "./graph/graph_utils.h"
# include <fstream>
using namespace std;

// File I/O
ifstream infile("input.txt");
ofstream outfile("output.txt");

void N2S_PATH1(int nodes, int edges, vector<vector<pair<int,int>>> &adj, int src, int dest, map<pair<int,int>,int> &edge_wt_map){
    vector<vector<int>> parent1(nodes);

    BFS(src, nodes, parent1, adj);

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

    // shortest path tree
    vector<int> parent2(nodes);
    SPT_DIJKSTRA(dest, nodes, parent2, adj);
    outfile << "Shortest-path Tree: " << '\n';
    for(int i=0; i<nodes; i++){
        outfile << "Node: " << i << ", " << "Parent: " << parent2[i] << '\n';
    }
    outfile << "----------- \n";

    vector<int> dist1(nodes,INT_MAX), dist2(nodes,INT_MAX);

    // dG(u,x) 
    DIJKSTRA(src, nodes, dist1, adj);

    // dG(x,v)
    DIJKSTRA(dest, nodes, dist2, adj);

    outfile << "Dijkstra Distances dG(u,x): " << '\n';
    for(int i=0; i<nodes; i++){
        outfile << "Node: " << i << ", " << "Distance: " << dist1[i] << '\n';
    }
    outfile << "----------- \n";

    outfile << "Dijkstra Distances dG(x,v): " << '\n';
    for(int i=0; i<nodes; i++){
        outfile << "Node: " << i << ", " << "Distance: " << dist2[i] << '\n';
    }
    outfile << "----------- \n";

    // Finding V'
    set<int> v_prime;

    for(auto it: edge_wt_map){
        int x = it.first.first, y = it.first.second;
        bool is_forward = (dist1[x] + it.second + dist2[y] == dist1[dest]);
        bool is_backward = (dist1[y] + it.second + dist2[x] == dist1[dest]);
        if(!is_forward && !is_backward){
            v_prime.insert(x);
        }
    }

    outfile << "v' = { ";
    for(auto it: v_prime){
        outfile << it << " ";
    }
    outfile << "}" << '\n';
    outfile << "----------- \n";

    vector<int> v(v_prime.begin(), v_prime.end());
    sort(v.begin(), v.end(), [&](int a, int b){
        return dist1[a] < dist1[b];
    });

    outfile << "sorted v' = { ";
    for(auto it: v){
        outfile << it << " ";
    }
    outfile << "}" << '\n';
    outfile << "----------- \n";

    vector<int> ans_set;

    vector<vector<int>> children(nodes);
    for(int i=0; i<nodes; i++){
        if(parent2[i]!=-1){
            children[parent2[i]].push_back(i);
        }
    }

    for(const auto& it: v){
        unordered_set<int> t_x;
        function<void(int)> buildTrimmedTree = [&](int node){
            t_x.insert(node);
            for(int child: children[node]){
                bool is_backward_arc = d_star[child].count(node) > 0;
                if(is_backward_arc) continue;
                buildTrimmedTree(child);
            }
        };

        buildTrimmedTree(it);
        int q = INT_MAX;
        vector<bool> visited(nodes, false);

        for(int node: t_x){
            for(const auto& [adj_node, weight]: adj[node]){
                if(t_x.count(adj_node) == 0 && !visited[adj_node]){
                    bool is_forward_arc = d_star[node].count(adj_node) > 0;
                    bool is_backward_arc = d_star[adj_node].count(node) > 0;
                    if(!is_forward_arc && !is_backward_arc){
                        int new_dist = dist2[node] - dist2[it] + edge_wt_map[{node, adj_node}] + dist2[adj_node];
                        q = min(q, new_dist);
                    }
                }
            }
            visited[node] = true;
        }

        if(q!= INT_MAX){
            ans_set.push_back(dist1[it]+q);
        }
    }

    outfile << "Length of simple path which is just greater than the shortest path: " << *min_element(ans_set.begin(), ans_set.end()) << '\n';
    outfile << "----------- \n";

}

int main() {

    // ifstream infile("input.txt");
    // ofstream outfile("output.txt");

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

    N2S_PATH1(n, e, adj, src, dest, edge_wt_map);

    outfile << "End of the program!" << '\n';
    outfile << "Thank you for using our BTP Project!" << '\n';

    infile.close();
    outfile.close();

    return 0;
}