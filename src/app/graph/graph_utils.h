#ifndef GRAPH_UTILS_H
#define GRAPH_UTILS_H

#include <bits/stdc++.h>
using namespace std;

void FIND_PATHS(int node, vector<int> &path, vector<vector<int>> &parent, set<vector<int>> &paths);
void BFS(int src, int n, vector<vector<int>> &parent, vector<vector<pair<int, int>>> &adj);
void SPT_DIJKSTRA(int root, int n, vector<int> &parent, vector<vector<pair<int,int>>> &adj);
void DIJKSTRA(int node, int n, vector<int> &dist, vector<vector<pair<int,int>>> &adj);

#endif // GRAPH_UTILS_H
