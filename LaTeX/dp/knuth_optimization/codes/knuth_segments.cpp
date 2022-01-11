\begin{code}
for(int ki = 1; ki <= k; ki++) {
    for(int ni = n - 1; ni >= 0; ni--) {
        int l = opt[ki - 1][ni]; // opt[0][ni] = 0
        int r = opt[ki][ni + 1]; // opt[ki][n] = n - 1
        for(int i = l; i <= r; i++) {
            if(dp[ki][ni] > dp[ki - 1][i] + w(i, ni)) {
                dp[ki][ni] = dp[ki - 1][i] + w(i, ni);
                opt[ki][ni] = i;
            }
        }
    }
}
\end{code}
