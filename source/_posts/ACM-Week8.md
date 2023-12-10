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

## P2661 [NOIP2015 提高组] 信息传递

### 思路
初始化一个数组，用于存储每个节点的状态。0表示未访问，1表示正在访问，2表示已访问。
对于每个节点，如果它未被访问，就从它开始进行深度优先搜索。
在深度优先搜索中，如果我们遇到一个正在访问的节点，那么我们就找到了一个环。我们可以通过当前节点的深度减去环开始节点的深度加1来计算环的长度。
更新游戏可以进行的最大轮数。

### 代码
```Cpp
#include<bits/stdc++.h>
using namespace std;
const int MAXN = 2e5+7;
int n, ans = 0;
int Next[MAXN], vis[MAXN], dis[MAXN];

void dfs(int x) {
    vis[x] = 1;
    if(!vis[Next[x]]) {
        dis[Next[x]] = dis[x] + 1;
        dfs(Next[x]);
    } else if(vis[Next[x]] == 1) {
        ans = max(ans, dis[x] - dis[Next[x]] + 1);
    }
    vis[x] = 2;
}

int main() {
    cin >> n;
    for(int i = 1; i <= n; i++) cin >> Next[i];
    for(int i = 1; i <= n; i++) {
        if(!vis[i]) {
            dis[i] = 1;
            dfs(i);
        }
    }
    cout << ans << endl;
    return 0;
}
```

## P1144 最短路计数

### 思路
BFS, DP

初始化两个数组，一个用于存储每个节点的最短距离，一个用于存储每个节点的最短路径数量。将所有节点的最短距离设置为无穷大，将节点1的最短路径数量设置为1。
使用广度优先搜索从节点1开始遍历图。对于每个访问的节点，如果它的最短距离大于当前节点的最短距离加1，那么更新它的最短距离和最短路径数量。如果它的最短距离等于当前节点的最短距离加1，那么增加它的最短路径数量。
输出每个节点的最短路径数量。

### 代码
```Cpp
#include<bits/stdc++.h>
using namespace std;
const int MAXN = 1e6+7;
const int MOD = 100003;
vector<int> edge[MAXN];
int dis[MAXN], cnt[MAXN];

void bfs() {
    queue<int> q;
    memset(dis, 0x3f, sizeof(dis));
    dis[1] = 0;
    cnt[1] = 1;
    q.push(1);
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        for(int i = 0; i < edge[u].size(); i++) {
            int v = edge[u][i];
            if(dis[u] + 1 < dis[v]) {
                dis[v] = dis[u] + 1;
                cnt[v] = cnt[u];
                q.push(v);
            } else if(dis[u] + 1 == dis[v]) {
                cnt[v] = (cnt[v] + cnt[u]) % MOD;
            }
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        edge[u].push_back(v);
        edge[v].push_back(u);
    }
    bfs();
    for(int i = 1; i <= n; i++) {
        cout << cnt[i] << endl;
    }
    return 0;
}
```

## [蓝桥杯 2022 国 A] 环境治理

### 思路
初始化两个数组，一个用于存储每个节点的最短距离，一个用于存储每个节点的最短路径数量。将所有节点的最短距离设置为无穷大，将节点1的最短路径数量设置为1。
使用广度优先搜索从节点1开始遍历图。对于每个访问的节点，如果它的最短距离大于当前节点的最短距离加1，那么更新它的最短距离和最短路径数量。如果它的最短距离等于当前节点的最短距离加1，那么增加它的最短路径数量。
输出每个节点的最短路径数量。

### 代码
```Cpp
#include<bits/stdc++.h>
using namespace std;
const int MAXN = 1e6+7;
const int MOD = 100003;
vector<int> edge[MAXN];
int dis[MAXN], cnt[MAXN];

void bfs() {
    queue<int> q;
    memset(dis, 0x3f, sizeof(dis));
    dis[1] = 0;
    cnt[1] = 1;
    q.push(1);
    while(!q.empty()) {
        int u = q.front();
        q.pop();
        for(int i = 0; i < edge[u].size(); i++) {
            int v = edge[u][i];
            if(dis[u] + 1 < dis[v]) {
                dis[v] = dis[u] + 1;
                cnt[v] = cnt[u];
                q.push(v);
            } else if(dis[u] + 1 == dis[v]) {
                cnt[v] = (cnt[v] + cnt[u]) % MOD;
            }
        }
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        edge[u].push_back(v);
        edge[v].push_back(u);
    }
    bfs();
    for(int i = 1; i <= n; i++) {
        cout << cnt[i] << endl;
    }
    return 0;
}
```