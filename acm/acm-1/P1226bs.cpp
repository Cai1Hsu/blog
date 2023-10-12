#include <bits/stdc++.h>
#include <cstdio>
#include "binarysearch.h"

int main()
{
  int n, m;

  cin >> n >> m;

  vector<int> arr(n, 0);

  for (int i = 0;i < n; i++)
    scanf("%d", &arr[i]);

  for (int i = 0; i < m; i++)
  {
    int target;
    cin >> target;

    int idx = binarysearch(arr, target);

    printf("%d ", idx);
  }
}
