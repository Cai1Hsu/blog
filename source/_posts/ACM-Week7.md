---
title: ACM-Week7
date: 2023-11-26 21:36:54
tags:
---

## P1162 填涂颜色

### 思路
BFS

### 代码
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

## B3625 迷宫寻路

### 思路
BFS

### 代码
```Cpp
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 105;
char maze[MAXN][MAXN];
bool visited[MAXN][MAXN];
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};
int n, m;

bool bfs(int x, int y) {
    queue<pair<int, int>> q;
    q.push({x, y});
    visited[x][y] = true;

    while (!q.empty()) {
        pair<int, int> p = q.front();
        q.pop();

        if (p.first == n - 1 && p.second == m - 1) {
            return true;
        }

        for (int i = 0; i < 4; i++) {
            int nx = p.first + dx[i];
            int ny = p.second + dy[i];

            if (nx >= 0 && nx < n && ny >= 0 && ny < m && !visited[nx][ny] && maze[nx][ny] == '.') {
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }

    return false;
}

int main() {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> maze[i][j];
        }
    }

    if (bfs(0, 0)) {
        cout << "Yes" << endl;
    } else {
        cout << "No" << endl;
    }

    return 0;
}
```

## P1706 全排列问题

### 思路
BFS

### 代码
```Cpp
#include <bits/stdc++.h>
using namespace std;

int a[10];
int main()
{
	int n,i,j=1;
	cin>>n;

	for(i=1;i<=n;i++) {
		a[i]=n-i+1;j*=i;
	}

	for(i=1;i<=j;i++)
	 {
		next_permutation(a+1,a+n+1);

		for(int k=1;k<=n;k++)
			cout<<"    "<<a[k];

	   cout<<endl;
	 }
	 return 0;
}
```


## P1451 求细胞数量

### 思路
DFS

### 代码
```Cpp
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 105;
char matrix[MAXN][MAXN];
bool visited[MAXN][MAXN];
int dx[4] = {0, 1, 0, -1};
int dy[4] = {1, 0, -1, 0};
int n, m;

void dfs(int x, int y) {
    visited[x][y] = true;

    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && nx < n && ny >= 0 && ny < m && !visited[nx][ny] && matrix[nx][ny] != '0') {
            dfs(nx, ny);
        }
    }
}

int main() {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> matrix[i][j];
        }
    }

    int cells = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (!visited[i][j] && matrix[i][j] != '0') {
                dfs(i, j);
                cells++;
            }
        }
    }

    cout << cells << endl;

    return 0;
}
```

## P1219 [USACO1.5] 八皇后 Checker Challenge

### 思路
BFS

### 代码
```Cpp
#include <bits/stdc++.h>
using namespace std;

const int MAXN = 15;
int queens[MAXN], cols[MAXN], diags1[2*MAXN], diags2[2*MAXN];
int n, solutions;

void solve(int row) {
    if (row == n) {
        if (solutions < 3) {
            for (int i = 0; i < n; i++) {
                if (i > 0) cout << " ";
                cout << queens[i] + 1;
            }
            cout << "\n";
        }
        solutions++;
        return;
    }

    for (int col = 0; col < n; col++) {
        if (!cols[col] && !diags1[row+col] && !diags2[row-col+n-1]) {
            queens[row] = col;
            cols[col] = diags1[row+col] = diags2[row-col+n-1] = 1;
            solve(row + 1);
            cols[col] = diags1[row+col] = diags2[row-col+n-1] = 0;
        }
    }
}

int main() {
    cin >> n;
    solve(0);
    cout << solutions << "\n";
    return 0;
}
```