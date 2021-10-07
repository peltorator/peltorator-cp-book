\begin{code}
vector<int> build_diffs_array(const vector<int>& arr) {
    int n = arr.size();
    vector<int> diffs(n - 1);
    for (int i = 0; i < n - 1; i++) {
        diffs[i] = arr[i + 1] - arr[i];
    }
    return diffs;
}
\end{code}
