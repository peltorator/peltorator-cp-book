\begin{code}
vector<vector<int>> build_prefix_sums_2d(const vector<vector<int>>& a) {
    int n = a.size();
    int m = a[0].size();
    vector<vector<int>> prefix_sum_1d(n);
    for (int i = 0; i < n; i++) {
        prefix_sum_1d[i] = build_prefix_sums(a[i]);
    }
    vector<vector<int>> prefix_sum_2d(n + 1, vector<int>(m + 1, 0));
    for (int j = 0; j <= m; j++) {
        for (int i = 0; i < n; i++) {
            prefix_sum_2d[i + 1][j] = prefix_sum_2d[i][j] + prefix_sum_1d[i][j];
        }
    }
    return prefix_sum_2d;
}
\end{code}
