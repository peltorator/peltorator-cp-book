\begin{code}
// find dp[ki][ni] for all ni: l <= ni < r if cl <= opt[ki][ni] < cr
void divide(int ki, int L, int R, int cl, int cr) {
    if (L >= R) { // empty segment
        return;
    }
    int M = (L + R) / 2;
    int opt; // optimal partition point for dp[ki][M]
    for (int i = cl; i < min(M, cr); i++) {
        if (dp[ki][M] > dp[ki - 1][i] + w(i, M)) {
            dp[ki][M] = dp[ki - 1][i] + w(i, M);
            opt = i;
        }
    }
    divide(ki, L, M, cl, opt + 1);
    divide(ki, M + 1, R, opt, cr);
}
\end{code}
