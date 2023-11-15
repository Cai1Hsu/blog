---
title: '[Biweekly-2] 补题 & 反思'
date: 2023-11-15 09:50:15
tags: ['算法', '菜']
---

## 博客仓库
[Cai1Hsu/blog](https://github.com/Cai1Hsu/blog)

## 代码链接
[acm/biweek-2](https://github.com/Cai1Hsu/blog/tree/master/acm/biweek-2)

## CI
[GitHub Actions](https://github.com/Cai1Hsu/blog/actions/workflows/build.yml)

## 1
非常典型的不完全背包。
```Cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, W;
    cin >> n >> W;

    vector<int> weights(n);
    vector<int> values(n);

    for (int i = 0; i < n; i++)
        cin >> weights[i] >> values[i];

    vector dp(W + 1, 0);

    for (int i = 0; i < n; i++) {
        for (int t = W; t >= weights[i]; t--) {
            dp[t] = max(dp[t], dp[t - weights[i]] + values[i]);
        }
    }

    cout << dp[W];
}
```
## 2

### 比赛时的解答
非常暴力的 dfs 加上简单的优化（把水平的2x1和2x2统一起来），耗时居然还能看。
最后还有个不知道哪来的 `ans / 3`。但是就是过了.
```Cpp
#include <bits/stdc++.h>
using namespace std;

const int SQUARE = 1;
const int VERTICLE = 2;
const int HORIZONTAL = 4;

long long dfs(const int n, const int did) {
    if (n == 1 || n == 0)
        return 1;

    if (did == VERTICLE)
        return dfs(n - 1, SQUARE) * 2
            + dfs(n - 1, VERTICLE);

    return dfs(n - 2, SQUARE) * 2
            + dfs(n - 2, VERTICLE);

}

int main() {
    int T;
    cin >> T;

    for (int i = 0; i < T; i++) {
        int n;
        cin >> n;

        cout << dfs(n + 2, -1) / 3;

        // 抽象的 i != n - 1
        // 还好没有测试点把这个typo拦下来
        if (i != n - 1)
            cout << endl;
    }
}
```

### DP
从第三个瓷砖开始，我们可以在铺i - 1的基础上铺一个1x2的瓷砖，也可以在铺i - 2的基础上铺一个2x2或者2x1。考虑到铺了一个2x1就必须再铺一个2x1，我们可以将这两种情况统一起来。
```Cpp
#include <bits/stdc++.h>
#include <vector>
using namespace std;

long long solve(int n)
{
  vector<long long> dp(n + 1, 1);

  for (int i = 2; i <= n; i++)
  {
    dp[i] = dp[i - 1] + dp[i - 2] * 2;
  }

  return dp[n];
}

int main()
{
  int T;
  cin >> T;

  for (int i = 0; i < T; i++)
  {
    int n;
    cin >> n;

    cout << solve(n);

    if (i != T - 1)
      cout << endl;
  }

}
```
## 3
```
throw new NullReferenceException();
```

## 4
比赛时的代码写的有点烂，我自己都看不下去。重新写了一份。
```Csharp
var input = Console.ReadLine()!;

var n = int.Parse(Console.ReadLine()!);

string ans = input;
bool found = false;

for (int i = 0; i < n; i++)
{
    var current = Console.ReadLine()!;

    if (current.Length < input.Length)
        continue;

    if (!current.StartsWith(input))
        continue;

    if (string.Compare(current, ans) < 0)
        ans = current;

    if (!found)
        ans = current;

    found = true;
}

Console.WriteLine(ans);
```
## 5
```
throw new LimitedAbilityException();
```

## 6
```
throw new LimitedAbilityException();
```
