using System;
using System.Linq;

namespace Solution
{
    class Program
    {
        static void Main(string[] _)
        {
            int n = int.Parse(Console.ReadLine());

            long[,] dp = new long[n + 2, n + 2];

            // If we dont push any element, we can't pop
            for (int popped = 0; popped <= n; popped++)
            {
                dp[0, popped] = 1;
            }

            for (int pushed = 1; pushed <= n; pushed++ )
            {
                for (int popped = pushed; popped <= n; popped++)
                {
                    if (pushed == popped)
                        dp[pushed, popped] = dp[pushed - 1, popped];
                    else
                        dp[pushed, popped] = dp[pushed - 1, popped] + dp[pushed, popped - 1];
                }
            }

            Console.WriteLine(dp[n, n]);
        }
    }
}