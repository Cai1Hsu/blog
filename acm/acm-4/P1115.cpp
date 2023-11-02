#include <bits/stdc++.h>
using namespace std;

int main()
{
  int n;
  cin >> n;

  int queue;
  cin >> queue;

  int ans = queue;

  for (int i = 1; i < n; i++)
  {
    int t;
    cin >> t;

    queue = max(t, queue + t);
    ans = max(ans, queue);
  }

  cout << ans;
}
