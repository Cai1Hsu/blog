---
title: '[ACM-Week6] 预备训练'
date: 2023-11-13 10:38:45
tags: ['摆烂']
---

## P1135 奇怪的电梯
### 思路
BFS
### 代码
```Cpp
#include <bits/stdc++.h>
using namespace std;

int main()
{
    int N, current, targe;
    int ans = 0x7fffffff;

    cin >> N >> current >> targe;

    vector<int> deltas(N);
    vector<bool> visited(N + 10, false);

    vector<int> leastOps(N + 10, -1);
    leastOps[current] = 0;

    for (auto &e : deltas)
        cin >> e;

    queue<int> pendingFloors;
    pendingFloors.push(current);

    while (!pendingFloors.empty())
    {
        current = pendingFloors.front();
        pendingFloors.pop();

        auto move = [&] (int cur) -> void
        {
            if (cur < 1 || cur > N)
                return;

            if (visited[cur])
                return;

            visited[cur] = true;

            leastOps[cur] = leastOps[current] + 1;

            pendingFloors.push(cur);
        };

        move(current + deltas[current]);
        move(current - deltas[current]);
    }
}
```

### 两个疑问
1. 为什么这里要把移动过后的楼层加入到队列中
#### Ans
这是广度优先搜索（BFS）的核心部分。当我们访问一个节点（在这个问题中，节点是楼层）时，我们需要将所有未访问过的相邻节点（在这个问题中，相邻节点是可以通过按电梯按钮到达的楼层）加入到队列中。

在这个问题中，我们可以通过按电梯的上下按钮从当前楼层移动到其他楼层。所以，当我们访问一个楼层时，我们需要将通过按电梯按钮能够到达的所有未访问过的楼层加入到队列中。

这样，**我们就可以保证先访问的楼层的相邻楼层会比后访问的楼层的相邻楼层先被访问**，这就是“广度优先”。同时，我们也可以保证我们访问了所有可以通过按电梯按钮到达的楼层，这样我们就可以找到从起始楼层到目标楼层所需的最少按键次数。

##### Trick
本例中，`pendingFloors` 始终只有一个元素，我们可以使用一个int代替它

2. 如果我因为去过某一楼层而提前返回，为什么能够保证leastOps中的每一个元素都是最少的次数
#### Ans
这是 BFS 的特性，由于我们优先考虑尽可能多地遍历相邻节点，第一次访问某一节点时，当前遍历深度一定最浅，也就是本题中的操作数最少。

## P1443 马的遍历
## 思路同上

## 代码
```Cpp
#include <bits/stdc++.h>
#include <iomanip>

using namespace std;

int n, m, x, y;
vector<vector<int>> matrix;
vector<vector<bool>> visited;
struct Point
{
  int x;
  int y;
  int depth;
};

queue<Point> q;

void visit(int x, int y, int depth)
{
  if (x < 0 || y < 0 || x >= n || y >= m)
    return;

  if (visited[x][y])
    return;

  visited[x][y] = true;

  matrix[x][y] = depth;

  q.push(Point{ x, y, depth });
}

int main()
{
  cin >> n >> m >> x >> y;

  x--;
  y--;

  matrix = vector<vector<int>>(n, vector<int>(m, -1));

  matrix[x][y] = 0;

  visited = vector<vector<bool>>(n, vector<bool>(m, false));
  visited[x][y] = true;

  Point first = Point{ x, y, 0 };

  q.push(first);

  while(!q.empty())
  {
    Point cur = q.front();
    q.pop();

    visit(cur.x + 1, cur.y + 2, cur.depth + 1);
    visit(cur.x + 1, cur.y - 2, cur.depth + 1);
    visit(cur.x - 1, cur.y + 2, cur.depth + 1);
    visit(cur.x - 1, cur.y - 2, cur.depth + 1);

    visit(cur.x + 2, cur.y + 1, cur.depth + 1);
    visit(cur.x + 2, cur.y - 1, cur.depth + 1);
    visit(cur.x - 2, cur.y + 1, cur.depth + 1);
    visit(cur.x - 2, cur.y - 1, cur.depth + 1);
  }

  for (auto &row : matrix)
  {
    for (auto e : row)
      cout << setw(5) << left << e;

    cout << endl;
  }
}
```

## P3958 奶酪
```Cpp
#include <bits/stdc++.h>
using namespace std;

int n, matrix[35][35];
bool visited[35][35];

void dfs(int i, int j)
{
    if (matrix[i][j] != 0 || visited[i][j] == true)
        return;

    if (i < 1 || i > n || j < 1 || j > n)
        return;

    visited[i][j] = true;

    dfs(i - 1, j);
    dfs(i + 1, j);
    dfs(i, j + 1);
    dfs(i, j - 1);
}

int main()
{
    cin >> n;

    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            cin >> matrix[i][j];

    // handle the four edges
    for (int i = 1; i <= n; i++)
    {
        dfs(1, i);
        dfs(n, i);
    }
    for (int i = 2/*cuz' we have already enumerated the cornor*/; i < n; i++)
    {
        dfs(i, 1);
        dfs(i, n);
    }

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (matrix[i][j] == 0)
                cout << (visited[i][j] ? 0 : 2) << " ";
            else
                cout << 1 << " ";
        }
        cout << "\n";
    }

    return 0;
}
```

## P1162 填涂颜色

### 代码
```Cpp
#include <bits/stdc++.h>
using namespace std;

int n, v[1005], t;
double x[1005], y[1005], z[1005], h, r;

inline bool is_connected(int i, int j)
{
    double dis = (x[i] - x[j]) * (x[i] - x[j])
                + (y[i] - y[j]) * (y[i] - y[j]) 
                + (z[i] - z[j]) * (z[i] - z[j]);

    return dis <= 4 * r * r;
}

bool ans;
void dfs(int k)
{
    if (ans)
        return;
    
    if (z[k] + r >= h)
    {
        ans = true;
        return;
    }
    
    v[k] = 1;
    
    for (int i = 1; i <= n; i++)
        if (!v[i] && is_connected(i, k))
            dfs(i);
}

int main()
{
    cin >> t;

    for (int i = 1; i <= t; i++)
    {
        memset(v, 0, sizeof(v));
        ans = false;

        cin >> n >> h >> r;

        for (int j = 1; j <= n; j++)
            cin >> x[j] >> y[j] >> z[j];

        for (int j = 1; j <= n; j++)
            if (z[j] - r <= 0)
                dfs(j);

        if (ans)
            cout << "Yes" << endl;
        else
            cout << "No" << endl;
    }
}
```