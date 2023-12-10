---
title: ACM-Week9
date: 2023-12-10 22:07:47
tags:
---

## P3367 【模板】并查集 

### 代码
```Cpp
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 1e4 + 5;
int parent[MAXN];

// 查找函数，用于查找元素 x 所在的集合
int find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

// 合并函数，用于将元素 x 和元素 y 所在的集合合并
void unionSet(int x, int y) {
    parent[find(x)] = find(y);
}

int main() {
    int n, m;
    cin >> n >> m;

    // 初始化并查集
    for (int i = 1; i <= n; i++) {
        parent[i] = i;
    }

    // 处理每个操作
    for (int i = 0; i < m; i++) {
        int z, x, y;
        cin >> z >> x >> y;
        if (z == 1) {
            // 合并操作
            unionSet(x, y);
        } else if (z == 2) {
            // 查询操作
            if (find(x) == find(y)) {
                cout << "Y" << endl;
            } else {
                cout << "N" << endl;
            }
        }
    }

    return 0;
}
```

## [蓝桥杯 2013 国 C] 危险系数

### 思路
使用深度优先搜索遍历图，同时记录每个节点的访问时间戳和最早可以回溯到的节点的时间戳。
在深度优先搜索的过程中，如果一个节点是其父节点可以回溯到的最早节点，并且该节点不是根节点，那么该节点就是一个割点。
对于根节点，如果它有两个或更多的子节点，那么它就是一个割点。
对于每一对需要查询的节点，如果它们在深度优先搜索树中是父子关系，并且子节点是一个割点，那么危险系数就是1，否则危险系数就是0。

### 代码
```Cpp
#include<bits/stdc++.h>
using namespace std;
const int MAXN = 1e3+7;
vector<int> edge[MAXN];
int dfn[MAXN], low[MAXN], cut[MAXN], Time;
void dfs(int u, int fa) {
    dfn[u] = low[u] = ++Time;
    int child = 0;
    for(int i = 0; i < edge[u].size(); i++) {
        int v = edge[u][i];
        if(!dfn[v]) {
            child++;
            dfs(v, u);
            low[u] = min(low[u], low[v]);
            if(low[v] >= dfn[u]) cut[u] = true;
        } else if(v != fa) {
            low[u] = min(low[u], dfn[v]);
        }
    }
    if(fa < 0 && child == 1) cut[u] = false;
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
    for(int i = 1; i <= n; i++) {
        if(!dfn[i]) dfs(i, -1);
    }
    int u, v;
    cin >> u >> v;
    int ans = 0;
    for(int i = 1; i <= n; i++) {
        if(cut[i]) ans++;
    }
    cout << ans << endl;
    return 0;
}
```

## P1330 封锁阳光大学

### 思路
将每个节点的父节点存储在一个数组中，并使用另一个数组存储每个节点的深度。然后我们可以使用深度优先搜索从节点1开始遍历图，更新每个节点的父节点和深度。在深度优先搜索的过程中，如果我们遇到一个已经访问过的节点，并且这个节点不是当前节点的父节点，那么我们就找到了一个环。我们可以通过并查集来合并环中的所有节点，使它们成为一个超级节点。最后，我们可以通过计算超级节点的数量来得到需要的河蟹的数量。

### 代码
```Cpp
#include<bits/stdc++.h>
using namespace std;
const int MAXN = 1e4+7;
vector<int> edge[MAXN];
int fa[MAXN], dep[MAXN], vis[MAXN], cir[MAXN];
int find(int x) {
    return fa[x] == x ? x : fa[x] = find(fa[x]);
}
void dfs(int u) {
    vis[u] = 1;
    for(int i = 0; i < edge[u].size(); i++) {
        int v = edge[u][i];
        if(!vis[v]) {
            dep[v] = dep[u] + 1;
            dfs(v);
        } else if(vis[v] == 1 && dep[u] > dep[v]) {
            for(int j = u; j != v; j = fa[j]) {
                fa[find(j)] = find(v);
            }
        }
    }
    vis[u] = 2;
}
int main() {
    int n, m;
    cin >> n >> m;
    for(int i = 1; i <= n; i++) {
        fa[i] = i;
    }
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        edge[u].push_back(v);
        edge[v].push_back(u);
    }
    for(int i = 1; i <= n; i++) {
        if(!vis[i]) dfs(i);
    }
    for(int i = 1; i <= n; i++) {
        cir[find(i)] = 1;
    }
    int ans = 0;
    for(int i = 1; i <= n; i++) {
        ans += cir[i];
    }
    cout << ans << endl;
    return 0;
}
```

## 图的遍历

### 思路
初始化一个数组，用于记录每个节点能到达的编号最大的节点。
从编号最大的节点开始进行深度优先搜索，对于每个节点，我们都记录下在其子树中编号最大的节点。
输出每个节点能到达的编号最大的节点。

### 代码
```Cpp
#include<bits/stdc++.h>
using namespace std;
const int MAXN = 1e5+7;
vector<int> edge[MAXN];
int ans[MAXN], vis[MAXN];
void dfs(int u) {
    ans[u] = u;
    vis[u] = 1;
    for(int i = 0; i < edge[u].size(); i++) {
        int v = edge[u][i];
        if(!vis[v]) {
            dfs(v);
        }
        ans[u] = max(ans[u], ans[v]);
    }
}
int main() {
    int n, m;
    cin >> n >> m;
    for(int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        edge[u].push_back(v);
    }
    for(int i = n; i >= 1; i--) {
        if(!vis[i]) dfs(i);
    }
    for(int i = 1; i <= n; i++) {
        cout << ans[i] << " ";
    }
    cout << endl;
    return 0;
}
```

## P1119 灾后重建

## 思路
初始化一个二维数组，用于存储每一天，每两个村庄之间的最短路径。
使用Floyd-Warshall算法来找出在每一天，每两个村庄之间的最短路径。
对于每个询问，输出在指定的天数，从指定的村庄到指定的村庄的最短路径。

## 代码
```Cpp
#include<bits/stdc++.h>
using namespace std;
const int MAXN = 207;
const int INF = 1e9+7;
int g[MAXN][MAXN], t[MAXN], d[MAXN][MAXN];
int main() {
    int n, m;
    cin >> n >> m;
    for(int i = 1; i <= n; i++) {
        cin >> t[i];
        for(int j = 1; j <= n; j++) {
            if(i == j) g[i][j] = 0;
            else g[i][j] = INF;
        }
    }
    for(int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        u++; v++;
        g[u][v] = g[v][u] = min(g[u][v], w);
    }
    for(int k = 1; k <= n; k++) {
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= n; j++) {
                g[i][j] = min(g[i][j], g[i][k] + g[k][j]);
            }
        }
    }
    int q;
    cin >> q;
    while(q--) {
        int x, y, t;
        cin >> x >> y >> t;
        x++; y++;
        if(t[x] > t || t[y] > t) {
            cout << "-1\n";
        } else {
            int ans = INF;
            for(int i = 1; i <= n; i++) {
                if(t[i] <= t) {
                    ans = min(ans, g[x][i] + g[i][y]);
                }
            }
            if(ans == INF) cout << "-1\n";
            else cout << ans << "\n";
        }
    }
    return 0;
}
```