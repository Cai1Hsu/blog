#include <bits/stdc++.h>
using namespace std;

const int SQUARE = 1;
const int VERTICLE = 2;
const int HORIZONTAL = 4;

long long dfs(const int n, const int did) {
    if (n == 1 || n == 0)
        return 1;

    if (did == VERTICLE)
        return dfs(n - 1, SQUARE) * 2
            + dfs(n - 1, VERTICLE);

    return dfs(n - 2, SQUARE) * 2
            + dfs(n - 2, VERTICLE);

}

int main() {
    int T;
    cin >> T;

    for (int i = 0; i < T; i++) {
        int n;
        cin >> n;

        cout << dfs(n + 2, SQUARE) / 3;

        if (i != n - 1)
            cout << endl;
    }
}
