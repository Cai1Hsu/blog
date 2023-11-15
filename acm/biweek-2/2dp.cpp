#include <bits/stdc++.h>
#include <vector>
using namespace std;

long long solve(int n)
{
  if (n == 1)
    return 1;

  vector<long long> dp(n + 1, 1);

  for (int i = 2; i <= n; i++)
  {
    dp[i] = dp[i - 1] + dp[i - 2] * 2;
  }

  return dp[n];
}

int main()
{
  int T;
  cin >> T;

  for (int i = 0; i < T; i++)
  {
    int n;
    cin >> n;

    cout << solve(n);

    if (i != T - 1)
      cout << endl;
  }

}
