#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int dp(int n, int k, int a[][k + 1])
{
    if (a[n][k] != -1)
        return a[n][k];
    if (k == 0 || k == 1)
        return k;
    if (n == 1)
        return k;
    int min = 1000000000;

    for (int x = 1; x <= k; x++)
    {
        int p1 = dp(n - 1, x - 1, a);
        int p2 = dp(n, k - x, a);

        int temp = 1+(p1 > p2 ? p1 : p2);
        if (temp < min)
            min = temp;
    }
    a[n][k] = min;
    return min;

    
}

int main()
{
    int n, k;
    scanf("%d %d", &n, &k);
    int a[n + 1][k + 1];
    for (int i = 0; i <= n; i++)
    {
        for (int j = 0; j <= k; j++)
            a[i][j] = -1;
    }
    int ans = dp(n, k, a);
    printf("%d", ans);
    return 0;
}
