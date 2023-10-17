#include <stdio.h>
#include <math.h>

bool isPrime(int x)
{
	int upper = __builtin_sqrt(x);

	for (int i = 2; i <= upper; i++)
	{
		if (x % i == 0)
			return false;
  	}

  return true;
}

int stk[10] = {-1}; // 定义一个长度为 10 的栈，初始值为 -1
bool isPalindrome(int x) // 判断一个整数是否为回文数
{
	int p = -1, backup = x; // 定义一个指针 p 和一个备份变量 backup，初始值分别为 -1 和 x

	// 将 x 的每一位数字压入栈中
	while (x > 0)
	{
		int digit = x % 10; // 取出 x 的个位数字

		stk[++p] = digit; // 将数字压入栈中

		x /= 10; // 将 x 的个位数字去掉
	}

	x = backup; // 恢复 x 的值
	// 依次取出 x 的每一位数字，并与栈中的数字比较
	while (x > 0)
	{
		int digit = x % 10; // 取出 x 的个位数字

		if (digit != stk[p]) // 如果数字不相等，则 x 不是回文数
			return false;

		p--; // 指针 p 向前移动一位
		x /= 10; // 将 x 的个位数字去掉
	}

	return true; // 如果所有数字都相等，则 x 是回文数
}


int main()
{
	int i, b;
	scanf("%d %d", &i, &b);

	i = i % 2 == 0 ? i + 1 : i;

	for (; i <= b; i += 2)
		i <= 9989899 && isPalindrome(i) && isPrime(i) && printf("%d\n", i);
}
