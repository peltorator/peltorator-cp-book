\begin{code}
vector<int> findPrefixSums(const vector<int>& a) {
    int n = a.size();
    vector<int> prefixSums(n + 1, 0);
    partial_sum(a.begin(), a.end(), prefixSums.begin() + 1);
    return prefixSums;
}
\end{code}
