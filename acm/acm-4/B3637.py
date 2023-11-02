n = int(input())
arr = list(map(lambda s: int(s), input().split(' ')))
dp = [1] * n

ans = 1

for i in range(0, n):
    for j in range(0, i):
        if arr[i] > arr[j]:
            dp[i] = max(dp[i], dp[j] + 1)
            ans = max(ans, dp[i])

print(ans)