#include <stdio.h>

int dp_soln(int n, int k, int dp[][k + 1])
{
    int m;
    for (m = 1; dp[m - 1][n] < k; m++)
    {
        for (int x = 1; x <= n; x++)
            dp[m][x] = 1 + dp[m - 1][x - 1] + dp[m - 1][x]; // dp[m][x] = 1 + dp[m - 1][x - 1] + dp[m - 1][x - 1];
    }
    return m - 1; // m - 1 because m is incremented one extra time
}


int main()
{
    int n, k;
    scanf("%d %d", &n, &k);
    int dp[k + 1][n + 1];
    for (int x = 0; x <= n; x++)
        dp[0][x] = 0; // 0 trials for 0 eggs
    int ans = dp_soln(n, k, dp);
    printf("%d", ans);
    return 0;
}
