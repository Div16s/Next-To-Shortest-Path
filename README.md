# Next-to-Shortest Path Algorithms (N2S-PATH1 & N2S-PATH2)

This project implements efficient algorithms to compute the **next-to-shortest path** between two vertices in a weighted undirected graph. The algorithms are based on the O(n²) approach described by Kao et al., and are demonstrated on a sample graph (see images in this repository).

---

## Problem Statement

Given a weighted undirected graph **G = (V, E, w)** and two vertices **u** and **v**, the goal is to find the shortest path from **u** to **v** that is strictly longer than the shortest path (i.e., the next-to-shortest path).

---

## Algorithms Implemented

### 1. N2S-PATH1 (Outward Edge Case)
- Finds next-to-shortest paths that deviate from shortest paths via an **outward edge** (an edge not present in any shortest path).

### 2. N2S-PATH2 (Backward Edge Case)
- Finds next-to-shortest paths that introduce a **backward edge** (an edge that reverses a direction in the derived shortest-path graph).

Both algorithms rely on:
- **D\***: The derived graph containing all edges that are part of at least one shortest path from **u** to **v**.
- **Shortest-Path Tree**: Built using Dijkstra’s algorithm from the destination vertex.

---

## Example Graphs

Sample graphs used for demonstration are included as images in this repository:
- **Graph G**: The original weighted undirected graph.
- ![Graph-G](https://github.com/Div16s/Next-To-Shortest-Path/blob/main/images/Graph-G.png?raw=true)
- **Derived Graph D\***: Shows only edges that are part of shortest paths.
- ![Graph-D_star](https://github.com/Div16s/Next-To-Shortest-Path/blob/main/images/Graph-D_star.png?raw=true)
- **Shortest-Path Tree**: Rooted at the destination vertex.
- ![Graph-Shortest_Path_Tree](https://github.com/Div16s/Next-To-Shortest-Path/blob/main/images/Graph-Shortest_Path_Tree.png?raw=true)

**Source (`u`) = 12, Destination (`v`) = 13**

---

## How It Works

1. **Construct D\***: Identify all edges belonging to any shortest path between `u` and `v`.
2. **Build Shortest-Path Tree**: Use Dijkstra’s algorithm to build the tree rooted at `v`.
3. **N2S-PATH1**:
    - For each candidate vertex, construct the trimmed subtree and look for the best outward edge to form a valid next-to-shortest path.
4. **N2S-PATH2**:
    - For each candidate, check for possible backward edges and compute the corresponding path length.
5. **Result**: The minimum path length found by both methods is the next-to-shortest path.

---

## Usage

### Input Format

- The input should be provided via `input.txt`.
- Format:

```plaintext
n e
u1 v1 w1
u2 v2 w2
...
ue ve we
src dest
```
Where:
- `n` = number of nodes (0-indexed)
- `e` = number of edges
- Each line after that represents an edge between node `u` and node `v` with weight `w`.
- Last line provides source (`src`) and destination (`dest`).

### Example
```plaintext
6 7
0 1 2
0 2 4
1 2 1
1 3 7
2 4 3
3 5 1
4 5 5
0 5
```

### Running the Project

Inside the `src/app/` directory:

```bash
# Compile
 g++ main.cpp graph/graph_utils.cpp -o program

# Run
 ./program.exe
```

The program reads the input graph from `input.txt` and outputs detailed information in `output.txt`, including all shortest paths, derived D* graph, shortest-path tree, and lengths of next-to-shortest paths found by N2S-PATH1 and N2S-PATH2.

---

## Output

The `output.txt` file will contain:
- List of all shortest paths from `src` to `dest`.
- Derived **D\*** graph.
- Shortest-Path Tree rooted at `dest`.
- Distances from source and destination.
- Identification of vertices for trimming trees.
- Length of the next-to-shortest path found.
- Step-by-step execution details.

---

## References

- "A Quadratic Algorithm for Finding Next-to-Shortest Paths in Graphs" by Kuo-Hua Kao, Tsan-sheng Hsu, Chien-Chung Shen, and Hong-Yi Wu (Algorithmica, 2011).

---
