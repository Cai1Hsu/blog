using System;
using System.Linq;

var vi = Console.ReadLine()!
                .Split(' ')
                .Select(int.Parse);

int n = vi.First(), m = vi.Last();

int[,] matrix = new int[n, m];

for (int i = 0; i < n; i++)
    matrix[i, 0] = 1;

for (int i = 0; i < m; i++)
    matrix[0, i] = 1;

for (int i = 1; i < n; i++)
{
    for (int j = 1; j < m; j++)
    {
        if (i % 2 != 1 || j % 2 != 1)
            matrix[i, j] = matrix[i - 1, j] + matrix[i, j - 1];
    }
}

Console.WriteLine(matrix[n - 1, m - 1]);