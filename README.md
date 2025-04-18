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

Sample graphs used for demonstration are included as hand-drawn images in this repository:
- **Graph G**: The original weighted undirected graph ([see image 3][3]).
- **Derived Graph D\***: Shows only edges that are part of shortest paths ([see image 1][1]).
- **Shortest-Path Tree**: Rooted at the destination vertex ([see image 2][2]).

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

- Adjacency list or edge list with weights.
- Specify source (`u`) and destination (`v`).

### Example (pseudo-code)
