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
        dp[i] = (dp[i] + dp[j]) % mod;
      }
    }

    cout << dp[n];
  }
