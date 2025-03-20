# include "graph_utils.h"

void FIND_PATHS(int node, vector<int> &path, vector<vector<int>> &parent, set<vector<int>> &paths){
    // base case
    if(node == -1){
        paths.insert(path);
        return;
    }

    for(auto par_node: parent[node]){
        path.push_back(node);
        FIND_PATHS(par_node, path, parent, paths);

        // backtrack
        path.pop_back();
    }
}

void BFS(int src, int n, vector<vector<int>> &parent, vector<vector<pair<int,int>>> &adj){
    queue<int> q;
    vector<int> dist(n, INT_MAX);

    q.push(src);
    parent[src] = {-1};
    dist[src] = 0;

    while(!q.empty()){
        int curr_node = q.front();
        q.pop();

        for(auto it: adj[curr_node]){
            int adj_node = it.first, edge_wt = it.second;
            if(dist[adj_node] > dist[curr_node]+edge_wt){
                dist[adj_node] = dist[curr_node]+edge_wt;
                q.push(adj_node);
                
                parent[adj_node].clear();
                parent[adj_node].push_back(curr_node);
            }
            else if(dist[adj_node] == dist[curr_node]+edge_wt){
                parent[adj_node].push_back(curr_node);
            }
        }
    }
}

void SPT_DIJKSTRA(int root, int n, vector<int> &parent, vector<vector<pair<int,int>>> &adj){
    priority_queue<pair<int,int>,vector<pair<int,int>>, greater<pair<int,int>>> pq;
    vector<int> dist(n, INT_MAX);

    pq.push({0,root});
    dist[root]=0;
    parent[root]=-1;
    
    while(!pq.empty()){
        auto it1 = pq.top();
        pq.pop();

        int curr_node = it1.second;
        for(auto it2: adj[curr_node]){
            int adj_node = it2.first, edge_wt = it2.second;
            if(dist[curr_node]+edge_wt < dist[adj_node]){
                dist[adj_node] = dist[curr_node]+edge_wt;
                parent[adj_node] = curr_node;
                pq.push({dist[adj_node],adj_node});
            }
        }
    }
}

