# include <bits/stdc++.h>
# include "./graph/graph_utils.h"
# include <fstream>
using namespace std;

// File I/O
ifstream infile("input.txt");
ofstream outfile("output.txt");

void N2S_PATH1(int nodes, int edges, vector<vector<pair<int,int>>> &adj, int src, int dest, map<pair<int,int>,int> &edge_wt_map){
    outfile << "\n----- N2S-PATH1 Algorithm -----\n";
    
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

    outfile << "Length of simple path which is just greater than the shortest path (PATH1): " << *min_element(ans_set.begin(), ans_set.end()) << '\n';
    outfile << "----------- \n";

}

void N2S_PATH2(int nodes, int edges, vector<vector<pair<int,int>>> &adj, int src, int dest, map<pair<int,int>,int> &edge_wt_map){
    outfile << "\n----- N2S-PATH2 Algorithm -----\n";

    vector<int> dist_from_src(nodes, INT_MAX);
    vector<int> dist_from_dest(nodes, INT_MAX);
    DIJKSTRA(src, nodes, dist_from_src, adj);
    DIJKSTRA(dest, nodes, dist_from_dest, adj);

    int shortest_path_length = dist_from_src[dest];
    outfile << "Shortest path length: " << shortest_path_length << "\n";

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

    vector<vector<int>> children(nodes);
    for(int i=0; i<nodes; i++){
        if(parent2[i]!=-1){
            children[parent2[i]].push_back(i);
        }
    }

    //Vertices in V* (vertices on shortest paths)
    set<int> V_star;
    for(int i = 0; i < nodes; i++) {
        if(!d_star[i].empty()) {
            V_star.insert(i);
            for(int j : d_star[i]) {
                V_star.insert(j);
            }
        }
    }
    
    // Computing idom(src, x) using BFS
    vector<int> idom(nodes, -1);
    vector<bool> visited(nodes, false);
    queue<int> q_bfs;
    q_bfs.push(src);
    visited[src] = true;
    idom[src] = src;
    
    while(!q_bfs.empty()) {
        int curr = q_bfs.front();
        q_bfs.pop();
        
        for(auto [next, _] : adj[curr]) {
            if(!visited[next]) {
                visited[next] = true;
                idom[next] = curr;
                q_bfs.push(next);
            }
        }
    }


    // Computing in-neighbors in D*
    vector<set<int>> N_minus_single(nodes);
    for(int i = 0; i < nodes; i++) {
        for(int j : d_star[i]) {
            N_minus_single[j].insert(i);
        }
        N_minus_single[i].insert(i);
    }

    outfile << "In-neighbors in D* graph:\n";
    for(int i = 0; i < nodes; i++) {
        outfile << "Node " << i << ": { ";
        for(int neighbor : N_minus_single[i]) {
            outfile << neighbor << " ";
        }
        outfile << "}\n";
    }
    outfile << "----------- \n";
    
    // Computing N^-[V(T_v(x))] for each vertex x in shortest path tree
    vector<set<int>> N_minus(nodes);
    
    function<void(int)> compute_N_minus = [&](int x) {
        // Base case: if x is a leaf node
        if(children[x].empty()) {
            N_minus[x] = N_minus_single[x];
            return;
        }
        
        for(int child : children[x]) {
            compute_N_minus(child);
        }
        
        N_minus[x] = N_minus_single[x];
        
        for(int child : children[x]) {
            N_minus[x].insert(N_minus[child].begin(), N_minus[child].end());
        }
    };
    
    compute_N_minus(dest);

    outfile << "N^-[V(T_v(x))] for each vertex:\n";
    for(int i = 0; i < nodes; i++) {
        outfile << "Node " << i << ": { ";
        for(int neighbor : N_minus[i]) {
            outfile << neighbor << " ";
        }
        outfile << "}\n";
    }
    outfile << "----------- \n";

    vector<int> ans_set = {INT_MAX};
    
    for(int y : V_star) {
        if(y == src || y == dest) continue;
        
        // Computing B = N^-[V(T_v(y))] \ (N^-[V(T_v(idom(u, y)))] ∪ {y})
        set<int> B;
        
        if(idom[y] != -1 && V_star.find(idom[y]) != V_star.end()) {
            set_difference(
                N_minus[y].begin(), N_minus[y].end(),
                N_minus[idom[y]].begin(), N_minus[idom[y]].end(),
                inserter(B, B.begin())
            );
        } else {
            B = N_minus[y];
        }
        B.erase(y);

        outfile << "B for y = " << y << ": { ";
        for(int b : B) {
            outfile << b << " ";
        }
        outfile << "}\n";
        outfile << "----------- \n";
        
        int q = INT_MAX;
        
        for(int w : B) {
            // If y ∉ Dom(w, v) by checking for paths from w to v that avoid y
            // y is not a dominator of w with respect to v
            queue<int> q_check;
            vector<bool> visited_check(nodes, false);
            visited_check[y] = true;
            q_check.push(w);
            visited_check[w] = (w == y);
            
            bool path_exists = false;
            while(!q_check.empty() && !path_exists) {
                int curr = q_check.front();
                q_check.pop();
                
                if(curr == dest) {
                    path_exists = true;
                    break;
                }
                
                for(int next : d_star[curr]) {
                    if(!visited_check[next]) {
                        visited_check[next] = true;
                        q_check.push(next);
                    }
                }
            }
            
            // If a path exists without y, then y ∉ D(w, v)
            if(path_exists) {
                int weight_y_w = edge_wt_map[{y, w}];
                if(weight_y_w == 0) {
                    weight_y_w = INT_MAX;
                }
                
                if(weight_y_w != INT_MAX) {
                    int path_length = dist_from_src[y] + weight_y_w + dist_from_dest[w];
                    
                    outfile << "Path from " << src << " to " << y << " to " << w << " to " << dest << ": " << path_length << "\n";
                    outfile << "Path length: " << path_length << "\n";
                    outfile << "----------- \n";

                    q = min(q, path_length);
                }
            }
        }
        
        ans_set.push_back(q);
    }
    
    int n2s_path_length = *min_element(ans_set.begin(), ans_set.end());
    outfile << "Length of simple path which is just greater than the shortest path (PATH2): " << n2s_path_length << "\n";
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
    N2S_PATH2(n, e, adj, src, dest, edge_wt_map);

    outfile << "End of the program!" << '\n';
    outfile << "Thank you for using our BTP Project!" << '\n';

    infile.close();
    outfile.close();

    return 0;
}