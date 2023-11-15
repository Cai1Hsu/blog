var input = Console.ReadLine()!;

var n = int.Parse(Console.ReadLine()!);

string ans = input;
bool found = false;

for (int i = 0; i < n; i++)
{
    var current = Console.ReadLine()!;

    if (current.Length < input.Length)
        continue;

    if (!current.StartsWith(input))
        continue;

    if (string.Compare(current, ans) < 0)
        ans = current;

    if (!found)
        ans = current;

    found = true;
}

Console.WriteLine(ans);