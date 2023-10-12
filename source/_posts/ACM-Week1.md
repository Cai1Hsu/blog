---
title: '[ACM 预备训练] Week1 2023.10.09 - 2023.10.15'
date: 2023-10-10 12:33:07
tags: ['算法', 'ACM']
---

## 代码源文件
Visit [Cai1Hsu/blog](https://github.com/Cai1Hsu/blog/tree/master/acm/acm-1)

## P8682 [蓝桥杯 2019 省 B] 等差数列
[题目链接](https://www.luogu.com.cn/problem/P8682)

### 题目描述
数学老师给小明出了一道等差数列求和的题目。但是粗心的小明忘记了一部分的数列，只记得其中 `N` 个整数。

现在给出这`N` 个整数，小明想知道包含这`N` 个整数的最短的等差数列有几项？

### 输入输出样例

#### 输入 #1
```
5
2 6 4 10 20
```

#### 输出 #1
```
10
```

### 思路
1. 要想找到有多少个数，先求出有公差。
2. 要保证数最少，则公差必须大。
3. 要构成等差数列，则公差 `d` 应满足 `d <= min{d1, d2, d3, ..., dn}`

### 代码 
```cpp
int main(int argc, char* argv[])
{
  int n = cint();

  vec<int> arr(n, 0);

  repeat(n, i) arr[i] = cint();

  sort(all(arr));

  int diff = 0x7fffffff;

  // 求出最小的 `gap`，即数列的最大公差
  range(1, n, i)
  {
    diff = min(diff, arr[i] - arr[i - 1]);
  }

  // 处理常数列，避免出现`divisor is equal to zero`
  int ans = arr.length;
  if (diff != 0)
  {
                                    // 向上取整
    ans = (arr.back() - arr.front() + (diff - 1)) / diff
            // 加上最后一个数
            + 1;
  }

  cout << ans;
}
```

## P1226 【模板】快速幂
[题目链接](https://www.luogu.com.cn/problem/P1226)

### 题目描述

给你三个整数 `a, b, p`，求 $`a^b mod p`。

### 输入格式

输入只有一行三个整数，分别代表 `a, b, p`。

### 输出格式

输出一行一个字符串 `a^b mod p=s`，其中 `a, b, p` 分别为题目给定的值， `s` 为运算结果。

### 样例 #1

#### 样例输入 #1

```
2 10 9
```

#### 样例输出 #1

```
2^10 mod 9=7
```

### 提示

**样例解释**

`2^10 = 1024`
`1024 mod 9 = 7`

**数据规模与约定**

对于 `100%` 的数据，保证 `0< a,b < 2^31`，`a + b > 0`，`2 <= p < 2^31`。

### 思路
1. 对于`a`, `b`,要求`a ^ b`，可以转化为`( a ^ (b / 2) ) ^ 2`。其中当`b`为奇数时，将结果再次乘以`a`即可。
2. 为了保证结果始终在`int`范围内，同时保证结果已经取余，需要在每次做乘法运算后取模。
3. 代码实现上，使用类似于`CPU`将乘法转化为加法的计算方法进行计算，我们将乘方的计算转化为乘法的计算，即：

- 考虑 `b` 的二进制表示:
```
bin: 1 0 1 0 1 1 1 0
dig: 8 7 6 5 4 3 2 1
```

- 则答案可以表示为：
```cpp
ans = a ^ (2 ^ 7) * 1 *
    a ^ (2 ^ 6) * 0 *
    a ^ (2 ^ 5) * 1 *
    a ^ (2 ^ 4) * 0 *
    a ^ (2 ^ 3) * 1 *
    a ^ (2 ^ 2) * 1 *
    a ^ (2 ^ 1) * 1 *
    a ^ (2 ^ 0) * 0;
```

- 从最低位开始计算，保存每一次`a ^ n`取模后的值，则`a ^ [2 ^ (n + 1)] = [a ^ (2 ^ n)] * [a ^ (2 ^ n)]`

- 综上，时间复杂度为 `O(1)`。

## 代码
```cpp
// 由 main() 读入
// int mod;

ll qpow(ll a, ll b)
{
    ll ans = 1;
        // 初始值 p = a ^ (2 ^ 0) = a ^ 1
        // 下一个 p = a ^ (2 ^ 1) = a ^ 2 = p1 * p1
        // 第三个 p = a ^ (2 ^ 2) = a ^ 4 = p2 * p2
    ll p = a % mod;

    range(0, 31, i)
    {
        // 表示当前 b 的比特位，即是否需要乘上当期的 p
        bool need_cur_p = (b >> i) & 1;

        if (need_cur_p)
            ans = ans * p % mod;

        p = p * p % mod;
    }

    return ans % mod;
}
```

## P2249 【深基13.例1】查找
[题目链接](https://www.luogu.com.cn/problem/P2249)

### 题目描述

输入 `n` 个不超过 `10^9` 的单调不减的（就是后面的数字不小于前面的数字）非负整数 `a[1], a[2], ..., a[n]`，然后进行 `m` 次询问。对于每次询问，给出一个整数 `q`，要求输出这个数字在序列中第一次出现的编号，如果没有找到的话输出 `-1` 。

### 输入格式

第一行 `2` 个整数 `n` 和 `m`，表示数字个数和询问次数。

第二行 `n` 个整数，表示这些待查询的数字。

第三行 `m` 个整数，表示询问这些数字的编号，从 `1` 开始编号。

### 输出格式

输出一行，`m` 个整数，以空格隔开，表示答案。

### 思路1
1. 由于 n <= 10 ^ 6，而`10 ^ 6 * 4 * 2 = 7.7 MB`，可以保证不会超出`125 MB`的限制，可以直接用散列表。

- 时间复杂度: `O(m)`（不考虑读入数据的时间）
- 空间复杂度 <= `O(n)`

### 代码1
本次提交使用了`fastscan()`代替`cint()`
```cpp
int main() {
    int n = cint(), m = cint();

    unordered_map<int, int> pos;
    
    crange(1, n, i) {
        int x = cint();
        if (!pos.count(x))
            pos[x] = i;
    }

    while (m--) {
        int x = cint();
        if (pos.count(x))
            cout << pos[x] << " ";
        else
            cout << "-1 ";
    }
}
```
### 思路2 二分查找
懒得写了

### 代码2 二分查找
```cpp
int binarysearch(const vector<int> &arr, int target)
{
  int left = 0, right = arr.size() - 1;

  while(left <= right)
  {
    int mid = left + (right - left) / 2;

    if (arr[mid] == target)
    {
      // 处理重复的元素
      while (mid >= 0 && arr[mid] == target)
        mid--;

      return mid + 2;
    }

    if (arr[mid] < target)
      left = mid + 1;
    else
      right = mid - 1;
  }

  return -1;
}
```


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
