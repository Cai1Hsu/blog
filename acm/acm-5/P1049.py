V = int(input())
n = int(input())

arr = [0] * n

for i in range(n):
    arr[i] = int(input())

dp = [0] * (V + 1)

# dp[w] = max(dp[w], dp[w - arr[i]] + arr[i])

ans = 0

for i in range(n):
    for w in range(V, arr[i] - 1, -1):
        dp[w] = max(dp[w], dp[w - arr[i]] + arr[i])
        ans = max(ans, dp[w])

print(V - ans)
