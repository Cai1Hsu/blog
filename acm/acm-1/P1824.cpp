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

int n, c;

vec<int> stalls;

bool check(int gap)
{
  int last = stalls[0];
  int count = 1; // We have already put one cow in the first stall

  range(1, n, i)
  {
    if (stalls[i] - last >= gap)
    {
      count++;
      last = stalls[i];
    }
  }

  return count >= c;
}

int binarysearch()
{
  int left = 0, right = stalls.back() - stalls.front();

  while (left < right)
  {
    int middle = (left + right + 1) >> 1;
    
    // 可行，继续往右边找，但要包含 middle
    if (check(middle))
    {
      left = middle;
    }
    // 不可行，往左边找
    else
    {
      right = middle - 1;
    }
  }

  return left;
}

int main()
{
  n = cint();
  c = cint();

  stalls = vec<int>(n + 1, 0);

  repeat(n, i) stalls[i] = cint();

  stalls.back() = 0x7ffffff;

  sort(all(stalls));

  int ans = binarysearch();

  cout << ans;
}
