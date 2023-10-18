---
title: '[ACM 预备训练] Week2' 
date: 2023-10-16 19:41:31
tags: ['ACM', '算法']
---

## 查看代码源文件
Visit [Cai1Hsu/blog](https://github.com/Cai1Hsu/blog/tree/master/acm/acm-2)
C/C++ 自动构建: [前往 GitHub](https://github.com/Cai1Hsu/blog/actions/workflows/build.yml)

## P1007 独木桥
[题目链接](https://www.luogu.com.cn/problem/P1007)

### 思路
1. 当两个士兵相遇时，同时转向可以理解为互相穿过。因此，我们只需要考虑桥上士兵到桥两端的距离

### 代码
```cpp
int min_time(const vec<int>& soldiers, int l)
{
  int ans = 0;

  foreach(s, soldiers)
  {
    if (s <= 0 || s > l)
      continue;

    ans = max(ans, min(s, l + 1 - s));
  }

  return ans;
}

int max_time(const vec<int>& soldiers, int l)
{
  int ans = 0;

  foreach(s, soldiers)
  {
    if (s <= 0 || s > l)
      continue;

    ans = max(ans, max(s, l + 1- s));
  }

  return ans;
}
```

### 拓展:
[Last Moment Before All Ants Fall Out of a Plank](https://leetcode.com/problems/last-moment-before-all-ants-fall-out-of-a-plank/)
没想到4年前做的题还能想起来

## P1223 排队接水
[题目链接](https://www.luogu.com.cn/problem/P1223)

## 思路
1. 要让所有人的平均等待时间最小，则等待总时间最少。
考虑总时间的表达方式：
```cpp
total_time = t[0] * n +
            t[1] * (n - 1) +
            t[2] * (n - 2) +
            ...
            t[n - 1] * 1
```
则t大的项应该尽量向后排，才能使每一项的和最小（甚至最大项不计入）。使得最后总和最小。
因此，我们将数组排序，从小到大，输出每一项的原下标`+1`即可。

## 代码
```cpp
bool cmp(pii a, pii b)
{
  return a.first < b.first;
}

int main()
{
  int n = cint();
  vec<pii> costs(n);

  repeat(n, i)
  {
    costs[i] = pii(cint(), i + 1);
  }

  sort(costs.begin(), costs.end(), cmp);

  ll sum = 0;

  foreach(p, costs)
  {
    sum += p.first * (--n);

    cout << p.second;

    if (p != costs.back()) cout << ' ';
  }

  cout << endl << fixed << setprecision(2) << (f64)sum / costs.size();
}
```

## P1803 凌乱的yyy / 线段覆盖
[题目链接](https://www.luogu.com.cn/problem/P1803)

### 思路
1. 首先按结束时间排序，因为我们想尽量参加完所有比赛
2. 然后依次遍历，判断能否参加即可。

### 代码
```cpp
#define tstart second
#define tend first
int main()
{
  int n = cint();

  vec<pii> durations(n);

  repeat(n, i)
    durations[i] = pii(cint(), cint());

  sort(durations.begin(), durations.end(), cmp);

  int cur = 0;
  int ans = 0;

  foreach(m, durations)
  {
    if (cur <= m.tstart)
    {
      ans++;
      cur = m.tend;
    }
  }

  cout << ans;
}
```

## P1031 [NOIP2002 提高组] 均分纸牌
[题目链接](https://www.luogu.com.cn/problem/P1031)

### 思路
1. 忽略已经处于平衡状态的堆。
2. 将当前堆的差异移动到下一组，在最后一组时一定使得所有堆平衡。
3. 统计操作数即可。

### 代码
```cpp
int main()
{
  int n = cint();

  vec<int> arr(n);

  int avg = 0;
  
  for(var &m : arr)
    avg += m = cint();

  avg /= n;

  for(var &m : arr)
    m -= avg;

  int op = 0;

  range(0, n - 1, i)
  {
    if (arr[i] == 0)
      continue;

    arr[i + 1] += arr[i];
    
    op++;
  }
  
  cout << op; 
}
```

## 博客仓库
[Cai1Hsu/blog](https://github.com/Cai1Hsu/blog)

## 使用的宏

```cpp
#include <bits/stdc++.h>
using namespace std;
#define add append
#define var auto
#define in :
#define all(a) begin(a), end(a)
#define rall(a) rbegin(a), rend(a)
#define pii pair<int, int>
#define length size()
#define mkpair make_pair
#define pow2(x) (1 << (x))
#define each(a, b) for (auto a : b)
#define foreach(a, b) for (auto a : b)
#define rsort(a) sort(a.rbegin(), a.rend())
#define null NULL
#define npr nullptr
#define MOD (100000007)
#define ll long long
#define ld long double
#define f32 float
#define f64 double
#define vi vector<int>
#define vec vector
#define range(begin,end, i) for(int i = begin; i < end; i++)
#define crange(begin,end, i) range(begin, end + 1, i)
#define repeat(times, i) range(0, times, i)
#define endl '\n'
void untie() { ios_base::sync_with_stdio(0); cin.tie(0); }
int cint() { int i; cin >> i; return i;}
```
