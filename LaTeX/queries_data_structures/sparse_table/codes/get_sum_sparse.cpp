\begin{code}
long long find_sum(int l, int r) { // [l, r)
    long long ans = 0;
    for (int power = logs[r - l]; power >= 0; power--) {
        if (l + (1 << power) <= r) {
            ans += sparse[power][l];
            l += (1 << power);
        }
    }
    return ans;
}
\end{code}
