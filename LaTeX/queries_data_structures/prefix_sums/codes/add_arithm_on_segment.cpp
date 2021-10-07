\begin{code}
vector<int> precalc(vector<int> b) {
    b.insert(b.begin(), 0);
    b.insert(b.begin(), 0); // add two leading zeros
    vector<int> a = build_diffs_array(build_diffs_array(b));
    return a;
}

// [l, r) += [step, 2 * step, ..., (r - l) * step]
void add_arithm_on_half_interval(int l, int r, int step) {
    a[l] += step;
    if (r < n) {
        a[r] -= (r - l + 1) * step;
    }
    if (r + 1 < n) {
        a[r + 1] += (r - l) * step;
    }
}

vector<int> postcalc(const vector<int>& a) {
    vector<int> finalb = build_prefix_sums(build_prefix_sums(a));
    finalb.erase(finalb.begin());
    finalb.erase(finalb.begin()); // delete leading zeros
    return finalb;
}
\end{code}
