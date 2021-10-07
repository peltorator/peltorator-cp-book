\begin{code}
vector<int> build_prefix_sums(const vector<int>& a) {
    int n = a.size();
    vector<int> prefix_sums(n + 1, 0);
    partial_sum(a.begin(), a.end(), prefix_sums.begin() + 1);
    return prefix_sums;
}
\end{code}
