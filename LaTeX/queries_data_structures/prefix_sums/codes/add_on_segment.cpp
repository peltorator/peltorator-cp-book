\begin{code}
vector<int> precalc(vector<int> b) {
    b.insert(b.begin(), 0); // add leading zero
    vector<int> a = build_diffs_array(b);
    return a;
}

void add_on_half_interval(int l, int r, int d) { // [l, r) += d
    a[l] += d;
    if (r < n) {
        a[r] -= d;
    }
}

vector<int> postcalc(const vector<int>& a) {
    vector<int> finalb = build_prefix_sums(a);
    finalb.erase(finalb.begin()); // delete leading zero
    return finalb;
}
\end{code}
