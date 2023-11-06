---
title: '[ACM-Week5] 五(-1)个难题'
date: 2023-11-06 11:13:42
tags: ['算法']
---

无法解决的五个难题。
但是，经过漫长的摆烂加上弱鸡的力量，那些问题
到现在也还是解不开。

## 难题「P1048 采药 - Easy」

### 思路同下

### 代码
```Python
totalItems, totalWeight = map(int, input().split())

dp = [0] * (totalItems + 1)
weights = [0] * (totalWeight + 1)
values = [0] * (totalWeight + 1)

for i in range(1, totalWeight + 1):
        (weights[i], values[i]) = map(int,input().split())

for i in range(1, totalWeight + 1):
        for j in range(totalItems, weights[i] - 1, -1):
                dp[j] = max(dp[j], dp[j - weights[i]] + values[i])

print(dp[totalItems])
```

## 难题「P1616 疯狂的采药 - Normal」
铃仙因为把自机放进永远亭而被永琳罚去采药

### 代码
```Python
t, m = map(int, input().split())

weights = [0] * m
values = [0] * m

dp = [0] * (t + 1)

for i in range(m):
   weights[i], values[i] = map(int, input().split())

# 遍历草药的种类
for j in range(m):
    # 从采集了w[j]草药所消耗的时间开始
    # 计算每个时间点所能采集的最大价值
    for i in range(weights[j], t + 1"""如果不加1时间就不会到最后，而是倒数第一分钟"""):
        # 最大值可能为原本的数值，即采集其他草药到达该时间点的最大价值
        # 也可能为上一个时间点后再采集当前草药j的价值
        dp[i] = max(dp[i], dp[i - weights[j]] + values[j])

print(dp[t])
```

## 难题 「P1049 装箱问题 - Hard」

### 思路同上

### 代码
```Python
V = int(input())
n = int(input())

arr = [0] * n

for i in range(n):
    arr[i] = int(input())

dp = [0] * (V + 1)

# dp[w] = max(dp[w], dp[w - arr[i]] + arr[i])

ans = 0

for i in range(n):
    for w in range(V, arr[i] - 1, -1):
        dp[w] = max(dp[w], dp[w - arr[i]] + arr[i])
        ans = max(ans, dp[w])

print(V - ans)
```

## 难题 「P1833 樱花 - Lunatic」

### 思路

### 代码


## 查看代码源文件
Visit [Cai1Hsu/blog](https://github.com/Cai1Hsu/blog/tree/master/acm/acm-5)
C/C++ 自动构建: [前往 GitHub](https://github.com/Cai1Hsu/blog/actions/workflows/build.yml)
