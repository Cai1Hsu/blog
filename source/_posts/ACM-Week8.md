---
title: ACM-Week8
date: 2023-12-03 21:21:58
tags:
---

## B3647 【模板】Floyd

### 思路
3. 初始化一个二维数组dist，其中dist[i][j]表示从顶点i到顶点j的最短距离。如果i和j之间有边，那么dist[i][j]就是这条边的权重，否则dist[i][j]就是无穷大。

2. 对于每个顶点k，我们尝试使用它作为所有顶点对的中间点。也就是说，我们尝试更新dist[i][j]为dist[i][k] + dist[k][j]，如果这个值比当前的dist[i][j]小的话。

1. 最后，dist数组就包含了所有顶点对之间的最短路径。
### 代码
```Python
n, m = map(int, input().split())
dist = [[float('inf')] * n for _ in range(n)]
for i in range(n):
    dist[i][i] = 0
for _ in range(m):
    u, v, w = map(int, input().split())
    dist[u-1][v-1] = w
    dist[v-1][u-1] = w
for k in range(n):
    for i in range(n):
        for j in range(n):
            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
for row in dist:
    print(' '.join(map(str, row)))
```

## P4779 【模板】单源最短路径（标准版）

### 思路
这个问题是一个经典的图论问题，可以使用Dijkstra算法来解决。Dijkstra算法是一个贪心算法，用于找出图中从一个顶点到所有其他顶点的最短路径。

以下是解决这个问题的步骤：

初始化一个数组dist，其中dist[i]表示从起点到顶点i的最短距离。起点到自身的距离为0，到所有其他顶点的距离为无穷大。

创建一个优先队列，用于存储待处理的顶点。起点是第一个待处理的顶点。

当优先队列非空时，取出距离最短的顶点，然后更新它的所有邻居的距离。

最后，dist数组就包含了从起点到所有其他顶点的最短路径。

### 代码
```Python
import heapq

n, m, s = map(int, input().split())
edges = [[] for _ in range(n+1)]
dist = [float('inf')] * (n+1)
dist[s] = 0
for _ in range(m):
    u, v, w = map(int, input().split())
    edges[u].append((v, w))
queue = [(0, s)]
while queue:
    _, u = heapq.heappop(queue)
    for v, w in edges[u]:
        if dist[u] + w < dist[v]:
            dist[v] = dist[u] + w
            heapq.heappush(queue, (dist[v], v))
print(' '.join(map(str, dist[1:])))
```