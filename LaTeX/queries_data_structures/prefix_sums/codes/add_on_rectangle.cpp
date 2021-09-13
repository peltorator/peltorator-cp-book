\begin{code}
// [lx, rx) * [ly, ry) += d
void addOnHalfRectangle (int lx, int ly, int rx, int ry, int d) {
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
    vector<vector<int>> finalArr = findPrefixSums2D(diffs);
    finalArr.erase(finalArr.begin());
    // delete leading horizontal zeros
    for (size_t i = 0; i < finalArr.size(); i++) {
        finalArr[i].erase(finalArr[i].begin());
        // delete leading vertical zeros
    }
    return finalArr;
}
\end{code}
