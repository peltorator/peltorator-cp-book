\begin{code}
vector<vector<int>> build_sparse_table(const vector<int>& arr) {
    int n = arr.size();
    int maxpow = ceil(log2(n + 1)); // 2^{maxpow} > n
    vector<vector<int>> sparse(maxpow, vector<int>(n, 0));
    sparse[0] = arr;
    for (int k = 0; k + 1 < maxpow; k++) {
        for (int ind = 0; ind + (1 << k) < n; ind++) {
            sparse[k + 1][ind] = min(sparse[k][ind], sparse[k][ind + (1 << k)]);
        }
    }
    return sparse;
}
\end{code}
