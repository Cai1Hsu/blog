n = int(input())
# 当然，[`cnt[i]`] 本身也是合法数列
cnt = [1] * (n + 1)

for i in range(1, n + 1):
    # 对于一个数`n`，所有的null, 1 ,2 ,3, ..., `n//2` 的合法数列前加上n都是合法数列
    for j in range(1, i // 2 + 1):
        cnt[i] += cnt[j]

print(cnt[n])
