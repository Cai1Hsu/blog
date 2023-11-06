n, t = map(int, input().split())

f=[0] * (n + 1)
c=[0] * (t + 1)
w=[0] * (t + 1)

for i in range(1, t + 1):
	(w[i], c[i]) = map(int,input().split())

for i in range(1, t+1):
	for j in range(n, w[i]-1, -1):
		f[j] = max(f[j], f[j - w[i]] + c[i])

print(f[n])
