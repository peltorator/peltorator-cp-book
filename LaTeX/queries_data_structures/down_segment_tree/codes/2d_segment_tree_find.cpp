\begin{code}
long long findSum(int lx, int rx, int ly, int ry) { // [lx, rx) * [ly, ry)
    lx += n;
    rx += n;

    long long ans = 0;
    while (lx < rx) {
        int curly = ly + m;
        int curry = ry + m;
        while (curly < curry) {
            if (curly & 1) {
                if (lx & 1) {
                    ans += tree[lx][curly];
                }
                if (rx & 1) {
                    ans += tree[rx - 1][curly];
                }
            }
            if (curry & 1) {
                if (lx & 1) {
                    ans += tree[lx][curry - 1];
                }
                if (rx & 1) {
                    ans += tree[rx - 1][curry - 1];
                }
            }
            curly = (curly + 1) >> 1;
            curry >>= 1;
        }
        lx = (lx + 1) >> 1;
        rx >>= 1;
    }
    return ans;
}
\end{code}
