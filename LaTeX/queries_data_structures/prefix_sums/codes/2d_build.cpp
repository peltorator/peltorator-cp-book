\begin{code}
vector<vector<int>> findPrefixSums2D(const vector<vector<int>>& a) {
    int n = a.size();
    int m = a[0].size();
    vector<vector<int>> prefixSum(n + 1, vector<int>(m + 1, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            prefixSum[i + 1][j + 1] = prefixSum[i][j + 1] + prefixSum[i + 1][j] - prefixSum[i][j] + a[i][j];
        }
    }
    return prefixSum;
}
\end{code}
