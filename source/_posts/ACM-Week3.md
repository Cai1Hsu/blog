---
title: '[ACM 预备训练] Week3'
date: 2023-10-23 11:01:21
tags: ['算法', '动态规划', 'DP']
---

## 查看代码源文件
Visit [Cai1Hsu/blog](https://github.com/Cai1Hsu/blog/tree/master/acm/acm-3)
C/C++ 自动构建: [前往 GitHub](https://github.com/Cai1Hsu/blog/actions/workflows/build.yml)

## Easy ~ P1028 [NOIP2001 普及组] 数的计算
[题目链接](https://www.luogu.com.cn/problem/P1028)

### 思路
- 见代码注释

### 代码
```Python
n = int(input())
# 0. cnt[i]用于储存`n = i`时的答案
# 2. 当然，[`cnt[i]`] 本身也是合法数列
cnt = [1] * (n + 1)

for i in range(1, n + 1):
    # 1. 对于一个数`n`，所有的null, 1 ,2 ,3, ..., `n//2` 的合法数列前加上n都是合法数列
    for j in range(1, i // 2 + 1):
        cnt[i] += cnt[j]

print(cnt[n])
```

## Normal ~ P1192 台阶问题
[题目链接](https://www.luogu.com.cn/problem/P1192)

### 思路
#### 核心思想
类似于数学归纳法的递推思想，对于上第n个台阶，我们可以考虑从`[n - k, n - 1]`中每一个台阶跳到n。
因此，我们只需记录`[1, n]`间每一个数的方法，然后求`[n - k, n - 1]`的和
#### 处理 edge case
- 当`i <= k`时，我们除了可以通过前面的台阶跳上去，还可以从起点直接跳上去。
- 其次，当`i <= k`时，要确保区间左端点大于等于`1`。

### 代码
```Cpp
#include <bits/stdc++.h>
using namespace std;

int main()
{
  const int mod = 100003;
  int n = 0, k = 0;
  cin >> n >> k;

  vector<int> dp(n + 1, 0);

  for (int i = 1; i <= n; i++)
  {
    if (i <= k)
      dp[i] = 1;

    for (int j = max(1, i - k); j < i; j++)
    {
      dp[i] += dp[j] % mod;
    }
  }

  cout << dp[n];
}
```
## Hard ~ P1044 [NOIP2003 普及组] 栈
[题目链接](https://www.luogu.com.cn/problem/P1044)

### 思路
创建一个 `dp[i,j]` 记录 当 *已经（总共）压入`i`个元素* 和 *已经（总共）弹出`j`个元素* 时的情况数。

### 代码
```Csharp
int n = int.Parse(Console.ReadLine()!);

long[,] dp = new(n + 2, n + 2);

// If we dont push any element, we can't pop
for (int popped = 0; popped <= n; popped++)
{
    dp[0, popped] = 1;
}

for (int pushed = 1; pushed <= n; pushed++)
{
    for (int popped = pushed; popped <= n; popped++)
    {
        if (pushed == popped)
            dp[pushed, popped] = dp[pushed - 1, popped];
        else
            dp[pushed, popped] = dp[pushed - 1, popped] + dp[pushed, popped - 1];
    }
}

Console.WriteLine(dp[n, n]);
```

### 关于遍历顺序的解释
The condition tpush == tpop checks if the stack is empty, in which case we can only push values onto the stack. Therefore, we set `dp[tpush, tpop] = dp[tpush - 1, tpop]` to update the value of `dp[tpush, tpop]`.

If the stack is not empty (`tpush < tpop`), then we can either push a value onto the stack (`dp[tpush - 1, tpop]`) or pop a value from the stack (`dp[tpush, tpop - 1]`). Therefore, we set `dp[tpush, tpop] = dp[tpush - 1, tpop] + dp[tpush, tpop - 1]` to update the value of `dp[tpush, tpop]`.

The reason for starting the inner loop at tpush is that the values of `dp[tpush - 1, tpop` are computed before the values of `dp[tpush, tpop - 1]`. This is because the values of tpush are enumerated in increasing order, so the value of tpush - 1 is always less than or equal to the value of tpush.

If we started the inner loop at `tpop = 1`, then we would need to compute the values of `dp[tpush, tpop - 1]` before the values of `dp[tpush - 1, tpop]`. This would require us to store the values of dp in a different order, which would make the algorithm more complicated.

Therefore, enumerating the values of tpush before the values of tpop and starting the inner loop at tpush ensures that the values of dp are computed correctly and simplifies the implementation of the algorithm.

## Lunatic ~ P1003 [NOIP2011 提高组] 铺地毯
[题目链接](https://www.luogu.com.cn/problem/P1003)
### 思路
- 先读入数据，将地毯全部储存起来
- 读入坐标
- 遍历地毯，如果被新的地毯覆盖，则更新答案

### 代码
感受一下Rust的优美
```Rust
use std::io::stdin;

#[derive(Debug)]
struct Rectangle {
    first: Point,
    second: Point
}

#[derive(Debug)]
struct Point {
    x: i32,
    y: i32
}

fn read_i32() -> i32 {
    let mut str = String::new();
    stdin().read_line(&mut str).unwrap();
    
    str.trim().parse::<i32>().unwrap()
}

fn read_point() -> Point {
    let mut str = String::new();
    stdin().read_line(&mut str).unwrap();

    let p: Vec<i32> = str.split(' ')
        .into_iter()
        .map(|s| s.trim().parse::<i32>().unwrap())
        .collect();
        
    Point {
        x: p[0],
        y: p[1],
    }
}

fn read_rectangle() -> Rectangle {
    let mut str = String::new();
    stdin().read_line(&mut str).unwrap();
    
    let vec4: Vec<i32> = str.split(' ')
        .into_iter()
        .map(|s| s.trim().parse::<i32>().unwrap())
        .collect();

    Rectangle { 
        first: Point {
            x: vec4[0], 
            y: vec4[1]

        },
        second: Point {
            x: vec4[0] + vec4[2],
            y: vec4[1] + vec4[3]
        }
    }
}

fn main() {
    let n: i32 = read_i32();

    let mut carpets: Vec<Rectangle> = Vec::new(); 
    for _ in 0..n {
        carpets.push(read_rectangle());
    }

    let target: Point = read_point();

    let mut ans: i32 = -1;

    for (i, rect) in carpets.iter().enumerate() {
        if rect.first.x <= target.x && 
            rect.first.y <= target.y && 
            rect.second.x >= target.x &&
            rect.second.y >= target.y
        {
            ans = i + 1; 
        }
    }

    println!("{}", ans);
}
```
## 博客仓库
[Cai1Hsu/blog](https://github.com/Cai1Hsu/blog)

