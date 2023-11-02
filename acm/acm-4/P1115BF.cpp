#include <bits/stdc++.h>
using namespace std;

int main()
{
  int n;
  cin >> n;

  vector<int> arr(n);

  for (int &e : arr)
    cin >> e;

  int ans = -2147483648;

  for (int i = 0; i < n; i++)
  {
    int sum = 0;
    for (int j = i; j > -1; j--)
    {
      sum += arr[j];
      
      ans = max(ans, sum);
    }
  }

  cout << ans;
}
