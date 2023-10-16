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

// https://leetcode.cn/problems/last-moment-before-all-ants-fall-out-of-a-plank/
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

int main()
{
  int l = cint(), n = cint();
  
  vec<int> soldiers(n);

  repeat(n, i)
    soldiers[i] = cint();

  int _min = min_time(soldiers, l);
  int _max = max_time(soldiers, l);

  printf("%d %d", _min, _max);
}

