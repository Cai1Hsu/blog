#include <bits/stdc++.h>
using namespace std;

int table[1250] = {2};
int len = 0;

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

void generatePrimeTable(int b)
{
	int upper = sqrt(b);

	for (int i = 0; i <= upper; i++)
		if (isPrime(i))
			table[++len] = i;
}

void process(int x)
{
	isPrime(x) && printf("%d\n", x);
}


int main()
{
	int a, b;
	cin >> a >> b;

	// 构造1位的回文数
	for (int d1 = 1; d1 <= 9; d1 += 2)
		process(d1);

	// 据说偶数位的回文质数只有 11
	process(11);

	// 构造3位的回文数
	for (int d1 = 1; d1 <= 9; d1 += 2)
        for (int d2 = 0; d2 <= 9; d2++)
            process(d1 * 100 + d2 * 10 + d1);
	
	// 构造5位的回文数
    for (int d1 = 1; d1 <= 9; d1 += 2)
        for (int d2 = 0; d2 <= 9; d2++)
            for (int d3 = 0; d3 <= 9; d3++) 
                process(d1 * 10000 + d2 * 1000 + d3 * 100 + d2 * 10 + d1);
	
	// 构造7位的回文数
	for (int d1 = 1; d1 <= 9; d1 += 2)
        for (int d2 = 0; d2 <= 9; d2++)
            for (int d3 = 0; d3 <= 9; d3++)
                for (int d4 = 0; d4 <= 9; d4++)
                    process(d1 * 1000000 + d2 * 100000 + d3 * 10000 + d4 * 1000 + d3 * 100 + d2 * 10 + d1);
}
