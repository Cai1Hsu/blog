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

ll mod = 0x7fffffff;

ll qpow(ll a, ll b)
{
    ll ans = 1;
        // 初始值 p = a ^ (2 ^ 0) = a ^ 1
        // 下一个 p = a ^ (2 ^ 1) = a ^ 2 = p1 * p1
        // 第三个 p = a ^ (2 ^ 2) = a ^ 4 = p2 * p2
    ll p = a % mod;

    range(0, 31, i)
    {
        bool need_cur_p = (b >> i) & 1;

        if (need_cur_p)
            ans = ans * p % mod;

        p = p * p % mod;
    }

    return ans % mod;
}

int main()
{
  ll a = cint(), b = cint();

  mod = cint();

  ll ans = qpow(a, b);

  printf("%d^%d mod %d=%d", a, b, mod, ans);
}
