---
title: '[ACM 预备训练] Week3'
date: 2023-10-23 11:01:21
tags: ['算法', '动态规划', 'DP']
---

## 查看代码源文件Visit [Cai1Hsu/blog](https://github.com/Cai1Hsu/blog/tree/master/acm/acm-3)
C/C++ 自动构建: [前往 GitHub](https://github.com/Cai1Hsu/blog/actions/workflows/build.yml)

## Easy ~ P1028 [NOIP2001 普及组] 数的计算
[题目链接](https://www.luogu.com.cn/problem/P1028)

### 思路
- 见代码注释

### 代码
```Python3
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
```Csharp

```
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

    let mut i: i32 = 1;
    for rect in carpets {
        if rect.first.x <= target.x && 
            rect.first.y <= target.y && 
            rect.second.x >= target.x &&
            rect.second.y >= target.y
        {
            ans = i; 
        }

        i += 1;
    }

    println!("{}", ans);
}
```
## 博客仓库
[Cai1Hsu/blog](https://github.com/Cai1Hsu/blog)

