\begin{code}
// [lx, rx) * [ly, ry) += d
void add_on_half_rectangle(int lx, int ly, int rx, int ry, int d) {
    diffs[lx][ly] += d;
    if (ry < m) {
        diffs[lx][ry] -= d;
    }
    if (rx < n) {
        diffs[rx][ly] -= d;
    }
    if (rx < n && ry < m) {
        diffs[rx][ry] += d;
    }
}

vector<vector<int>> postcalc(const vector<vector<int>>& diffs) {
    vector<vector<int>> final_arr = build_prefix_sums_2d(diffs);
    final_arr.erase(final_arr.begin());
    // delete leading horizontal zeros
    for (size_t i = 0; i < final_arr.size(); i++) {
        final_arr[i].erase(final_arr[i].begin());
        // delete leading vertical zeros
    }
    return final_arr;
}
\end{code}
