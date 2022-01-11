\begin{code}
for(int l = 0; l < n; l++) {
    for(int r = n; r > l; r--) {
        int from = (l == 0 ? 1 : max(opt[l - 1][r], l + 1));
        int to = (r == n ? n - 1 : min(opt[l][r + 1], r - 1));
        for(int i = from; i <= to; i++) {
            if(dp[l][r] > w(l, r) + dp[l][i] + dp[i][r]) {
                dp[l][r] = w(l, r) + dp[l][i] + dp[i][r];
                opt[l][r] = i;
            }
        }
    }
}
\end{code}
