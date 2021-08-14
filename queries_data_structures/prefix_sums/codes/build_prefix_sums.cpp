\begin{code}
vector<int> findPrefixSums(const vector<int>& a) {
    int n = a.size();
    vector<int> prefixSums(n + 1, 0);
    for (int i = 0; i < n; i++) {
        prefixSums[i + 1] = prefixSums[i] + a[i];
    }
    return prefixSums;
}
\end{code}
