// #include <bits/stdc++.h>
// using namespace std;

// int binarysearch(const vector<int> &arr, int target)
// {
//   int left = 0, right = arr.size() - 1;

//   while(left < right)
//   {
//     int mid = left + (right - left) / 2;

//     if (arr[mid] == target)
//       return mid + 1;

//     if (arr[mid] < target)
//       left = mid + 1;
//     else
//       right = mid - 1;
//   }

//   return -1;
// }


#include <bits/stdc++.h>
using namespace std;

int binarysearch(const vector<int> &arr, int target)
{
  int left = 0, right = arr.size() - 1;

  while(left <= right)
  {
    int mid = left + (right - left) / 2;

    if (arr[mid] == target)
    {
      while (arr[mid] == target)
        mid--;

      return mid + 2;
    }

    if (arr[mid] < target)
      left = mid + 1;
    else
      right = mid - 1;
  }

  return -1;
}