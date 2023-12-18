---
title: ACM-Week10
date: 2023-12-18 19:43:56
tags:
---

## P3386 【模板】二分图最大匹配
### 思路
初始化匹配数组 match 和访问数组 vis。
对于每一个左部点，如果它还没有匹配的右部点，就尝试用深度优先搜索找一个右部点。
在深度优先搜索中，对于每一个右部点，如果它还没有匹配的左部点，或者它的左部点可以匹配到其他的右部点，就更新匹配关系。

### 代码
```Cpp
#include<bits/stdc++.h>
using namespace std;
const int MAXN = 507;
vector<int> edge[MAXN];
int match[MAXN];
bool vis[MAXN];
bool dfs(int u) {
    for(int i = 0; i < edge[u].size(); i++) {
        int v = edge[u][i];
        if(!vis[v]) {
            vis[v] = true;
            if(match[v] == -1 || dfs(match[v])) {
                match[v] = u;
                return true;
            }
        }
    }
    return false;
}
int main() {
    int n, m, e;
    cin >> n >> m >> e;
    for(int i = 0; i < e; i++) {
        int u, v;
        cin >> u >> v;
        edge[u].push_back(v);
    }
    memset(match, -1, sizeof(match));
    int ans = 0;
    for(int i = 1; i <= n; i++) {
        memset(vis, false, sizeof(vis));
        if(dfs(i)) ans++;
    }
    cout << ans << endl;
    return 0;
}
```
## P1129 [ZJOI2007] 矩阵游戏
### 思路
这道题不能通过贪心算法来解决。我们不可以从最后一行开始，如果主对角线上的元素不是黑色，就在这一行以上找一个主对角线上是黑色的行来交换。如果找不到就说明无解。然后我们再处理列，方法同行。

从最后一行开始，如果主对角线上的元素不是黑色，就在这一行以上找一个主对角线上是黑色的行来交换。
如果在所有的行都处理完后，主对角线上还有非黑色的元素，就说明无解。
然后我们再处理列，方法同行。
### 代码 

```Cpp
#include<bits/stdc++.h>
using namespace std;
const int MAXN = 207;
int a[MAXN][MAXN];
int main() {
    int T;
    cin >> T;
    while(T--) {
        int n;
        cin >> n;
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= n; j++) {
                cin >> a[i][j];
            }
        }
        for(int i = n; i >= 1; i--) {
            if(a[i][i] == 0) {
                int j;
                for(j = i - 1; j >= 1; j--) {
                    if(a[j][i] == 1) {
                        swap(a[i], a[j]);
                        break;
                    }
                }
                if(j == 0) {
                    cout << "No\n";
                    goto END;
                }
            }
        }
        for(int i = n; i >= 1; i--) {
            if(a[i][i] == 0) {
                int j;
                for(j = i - 1; j >= 1; j--) {
                    if(a[i][j] == 1) {
                        for(int k = 1; k <= n; k++) {
                            swap(a[k][i], a[k][j]);
                        }
                        break;
                    }
                }
                if(j == 0) {
                    cout << "No\n";
                    goto END;
                }
            }
        }
        cout << "Yes\n";
        END:;
    }
    return 0;
}
```