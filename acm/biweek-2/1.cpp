#include <bits/stdc++.h>
using namespace std;

int main() {
    int n, W;
    cin >> n >> W;

    vector<int> weights(n);
    vector<int> values(n);

    for (int i = 0; i < n; i++)
        cin >> weights[i] >> values[i];

    vector dp(W + 1, 0);

    for (int i = 0; i < n; i++) {
        for (int t = W; t >= weights[i]; t--) {
            dp[t] = max(dp[t], dp[t - weights[i]] + values[i]);
        }
    }

    cout << dp[W];
}
