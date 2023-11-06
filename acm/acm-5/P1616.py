t, m = map(int, input().split())

weights = [0] * m
values = [0] * m

dp = [0] * (t + 1)

for i in range(m):
   weights[i], values[i] = map(int, input().split())

# 遍历草药的种类
for j in range(m):
    # 从采集了w[j]草药所消耗的时间开始
    # 计算每个时间点所能采集的最大价值
                            # 如果不加1时间就不会到最后，而是倒数第一分钟
    for i in range(weights[j], t + 1):
        # 最大值可能为原本的数值，即采集其他草药到达该时间点的最大价值
        # 也可能为上一个时间点后再采集当前草药j的价值
        dp[i] = max(dp[i], dp[i - weights[j]] + values[j])

print(dp[t])
