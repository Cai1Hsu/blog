using System;

static class Program
{
    static void Main(string[] args)
    {
        string input = Console.ReadLine();
        int n = int.Parse(Console.ReadLine().Trim());

        string ans = string.Empty;
        bool found = false;

        for (int i = 0; i < n; i++)
        {
            string s = Console.ReadLine();

            if (s.StartsWith(input))
            {
                if (!found)
                {
                    found = true;

                    ans = s;
                }
                else if (s.smaller(ans))
                {
                    ans = s;
                }
            }
        }
        
        if (found)
            Console.Write(ans);
        else
            Console.WriteLine(input);
    }

    static bool smaller(this string me, string other)
    {
        int upper = min(me.Length, other.Length);
        for (int i = 0; i < upper; i++)
        {
            if (me[i] > other[i])
                return false;

            if (me[i] < other[i])
                return true;
        }

        if (me.Length < other.Length)
            return true;

        return false;
    }

    static int min(int a, int b)
    {
        return a > b ? b : a;
    }
}
