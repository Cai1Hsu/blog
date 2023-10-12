#include <algorithm>
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

int cint() {
  bool negative = false;
  int c;
  int number = 0;
  c = getchar();
  if (c == '-')
  {
    negative = true;
    c = getchar();
  }
  for (; (c > 47 && c < 58); c = getchar())
    number = number * 10 + c - 48;
  if (negative)
    number *= -1;

  return number;
}

int main (int argc, char *argv[]) {
  int n = cint(), m = cint();

  // unordered_map<num, idx + 1>
  unordered_map<int, int> map;

  crange(1, n, i)
  {
    int num = cint();

    if (!map.count(num))
      map[num] = i;
  }

  repeat(m, i)
  {
    int target = cint();

    if (map.find(target) != map.end())
      printf("%d ", map[target]);
    else
      printf("-1 ");
  }

  return 0;
}
