\begin{code}
vector<int> build_prefix_sums(const vector<int>& a) {
    int n = a.size();
    vector<int> prefix_sums(n + 1, 0);
    for (int i = 0; i < n; i++) {
        prefix_sums[i + 1] = prefix_sums[i] + a[i];
    }
    return prefix_sums;
}
\end{code}
