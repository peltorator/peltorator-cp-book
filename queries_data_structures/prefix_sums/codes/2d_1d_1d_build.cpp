\begin{code}
vector<vector<int>> findPrefixSums2D(const vector<vector<int>>& a) {
    int n = a.size();
    int m = a[0].size();
    vector<vector<int>> prefixSum1D(n);
    for (int i = 0; i < n; i++) {
        prefixSum1D[i] = findPrefixSums(a[i]);
    }
    vector<vector<int>> prefixSum2D(n + 1, vector<int>(m + 1, 0));
    for (int j = 0; j <= m; j++) {
        for (int i = 0; i < n; i++) {
            prefixSum2D[i + 1][j] = prefixSum2D[i][j] + prefixSum1D[i][j];
        }
    }
    return prefixSum2D;
}
\end{code}
