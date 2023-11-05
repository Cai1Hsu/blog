---
title: '[ACM 预备训练] Week4'
date: 2023-10-30 10:44:34
tags: ['算法', 'ACM']
---

## 查看代码源文件
Visit [Cai1Hsu/blog](https://github.com/Cai1Hsu/blog/tree/master/acm/acm-4)
C/C++ 自动构建: [前往 GitHub](https://github.com/Cai1Hsu/blog/actions/workflows/build.yml)

## 题符 「B3637 最长上升子序列」~ Easy
[题目链接](https://www.luogu.com.cn/problem/B3637)

### 思路
1. 创建一个dp数组，记录`arr[0..=i]`间包含`arr[i]`的最大升序子序列
2. 记录数据时，我们选择一个快指针`i`,表示*子序列*的`最后一项`的下标，同时创建一个慢指针`j`,依次遍历`arr[0..i]`间的每一个值，如果`arr[i] > arr[j]`,说明`arr[i]`和`arr[j]`可以构成升序子序列。我们令`dp[i]` = `max(dp[j] + 1, dp[i])`，因为`dp[j]`是包含`arr[j]`的最大子序列的长度，而`arr[i]`可以排在该子序列后面。

- i, j 的状态如图:
```
[2, 1, 3, 8, 5, 6, 7, 4, 10]
 j              i
```

### 代码
```Python
n = int(input())
arr = list(map(lambda s: int(s), input().split(' ')))
dp = [1] * n

ans = 1

for i in range(0, n):
    for j in range(0, i):
        if arr[i] > arr[j]:
            dp[i] = max(dp[i], dp[j] + 1)
            ans = max(ans, dp[i])

print(ans)
```

## 题符 「P1115 最大子段和」~ Normal
[题目链接](https://www.luogu.com.cn/problem/P1115)

### 思路
1. 与上一道题相似，我们创建一个`dp[]`，其中`dp[i]`记录*包含i的最大子序列和*。
2. 这样定义的原因在于，当我们读取到下一个元素的时候，我们可以很容易判断是否需要加上下一个元素。因为如果`arr[i + 1] > dp[i] + arr[i + 1]`，我们应该让该子序列为`[arr[i + 1]]`，而其他时候则加上`arr[i + 1]`，同时，当和相等时，我们应尽量加上新的值，因为后面可能有机会使得整个序列的和更大。
3. 考虑到我们并不需要已经使用过的值（`arr[0..i+1]`和`dp[0..i+1`），我们可以考虑只使用常数大小的空间。

### 代码
```Cpp
#include <bits/stdc++.h>
using namespace std;

int main()
{
  int n;
  cin >> n;

  int queue;
  cin >> queue;

  int ans = queue;

  for (int i = 1; i < n; i++)
  {
    int t;
    cin >> t;

    queue = max(t, queue + t);
    ans = max(ans, queue);
  }

  cout << ans;
}
```

## 题符 「P8707 走方格」~ Hard
[题目链接](https://www.luogu.com.cn/problem/P8707)

### 思路
最简单DP题，对于每一个点`Matrix[i, j]`，走到它的方法为上面的点和左边的点的方法之和。
根据题意处理一下即可。

### Complaint
为什么不用.NET! 为什么不支持C# 12.0!
为什么不用.NET! 为什么不支持C# 12.0!
为什么不用.NET! 为什么不支持C# 12.0!
为什么不用.NET! 为什么不支持C# 12.0!
为什么不用.NET! 为什么不支持C# 12.0!
为什么不用.NET! 为什么不支持C# 12.0!
为什么不用.NET! 为什么不支持C# 12.0!
为什么不用.NET! 为什么不支持C# 12.0!

### 代码
```Csharp
var vi = Console.ReadLine()!
                .Split(' ')
                .Select(int.Parse);

int n = vi.First(), m = vi.Last();

int[,] matrix = new int[n, m];

for (int i = 0; i < n; i++)
    matrix[i, 0] = 1;

for (int i = 0; i < m; i++)
    matrix[0, i] = 1;

for (int i = 1; i < n; i++)
{
    for (int j = 1; j < m; j++)
    {
        if (i % 2 != 1 || j % 2 != 1)
            matrix[i, j] = matrix[i - 1, j] + matrix[i, j - 1];
    }
}

Console.WriteLine(matrix[n - 1, m - 1]);
```

## 题符 「P1216 数字三角形 Number Triangles」~ Lunatic
[题目链接](https://www.luogu.com.cn/problem/P1216)

## 思路
1. 我们从三角形的倒数第二行开始向上遍历。对于每一行，我们遍历每一个元素，将它更新为它自身加上它下一行的两个相邻元素中的较大者。这样，每个元素都会变成从它开始到底部的最大路径的和。
2. 当我们到达三角形的顶部时，triangle[0][0]就是我们要找的最大路径的和

## 代码
```Rust
use std::io::stdin;
use std::cmp::max;

fn read_vector() -> Vec<i32> {
    let mut str = String::new();
    stdin().read_line(&mut str).unwrap();

    str.split_whitespace()
        .map(|s| s.trim().parse::<i32>().unwrap_or(0))
        .collect()
}

fn main() {
    let n = read_vector()[0] as usize;

    let mut triangle: Vec<Vec<i32>> = Vec::new();

    for _ in 0..n {
        triangle.push(read_vector());
    }

    for i in (0..n-1).rev() {
        for j in 0..=i {
            triangle[i][j] = max(
                *triangle.get(i + 1).and_then(|row| row.get(j)).unwrap_or(&0),
                *triangle.get(i + 1).and_then(|row| row.get(j + 1)).unwrap_or(&0),
            );
        }
    }

    println!("{}", triangle[0][0]);
}
```

## 题符 「P1020 导弹拦截」~ Extra
[题目链接](https://www.luogu.com.cn/problem/P1020)

## 思路
1. 将题目抽象成模型: 最长不升子序列和最长上升子序列
2. 求解最长不升子序列。我们使用动态规划的方法，dp[i]表示以第i个导弹为结尾的最长不升子序列的长度。对于每一个导弹，我们都检查在它之前的所有导弹，如果某个导弹的高度大于或等于它，那么我们就可以选择在这个导弹之后发射当前的导弹，所以dp[i]就可以更新为dp[j] + 1（其中arr[j] >= arr[i]）。我们用一个变量ans来记录最长不升子序列的长度，每次更新dp[i]的时候，都用它来更新ans。

3. 求最长上升子序列。我们还是使用动态规划的方法，dp[i]表示以第i个导弹为结尾的最长上升子序列的长度。对于每一个导弹，我们都检查在它之前的所有导弹，如果某个导弹的高度小于它，那么我们就可以选择在这个导弹之后发射当前的导弹，所以dp[i]就可以更新为dp[j] + 1（其中arr[j] < arr[i]）。我们用一个变量ans来记录最长上升子序列的长度，每次更新dp[i]的时候，都用它来更新ans。

## 代码
```Cpp
#include<bits/stdc++.h>
using namespace std;

const int maxn = 100050;
int arr[maxn], dp[maxn], s[maxn], n = 0, k, ans;

void SearchAnswer(bool isGreater) {
    for(int i = 1; i <= n; ++i) {
        dp[i] = 1;
        int left = 0, right = ans + 1;

        while(right - left > 1) {
            int mid = (left + right) >> 1;

            if(isGreater) {
                // on the right side
                if(s[mid] >= arr[i]) {
                    left = mid;
                }
                // on the left side
                else {
                    right = mid;
                }
            } else {
                if(s[mid] < arr[i]) {
                    left = mid;
                }
                else {
                    right = mid;
                }
            }
        }

        dp[i] = left + 1;
        s[dp[i]] = arr[i];
        ans = max(ans, dp[i]);
    }
}

int main() {
    ios::sync_with_stdio(0);

    while(cin >> k)
        arr[++n] = k;

    SearchAnswer(true);
    cout << ans << endl;
    
    ans = 0;
    memset(s, 0, sizeof(s));

    SearchAnswer(false);
    cout << ans << endl;

    return 0;
}
```
## 博客仓库
[Cai1Hsu/blog](https://github.com/Cai1Hsu/blog)