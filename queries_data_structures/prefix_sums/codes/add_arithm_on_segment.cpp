\begin{code}
vector<int> precalc(vector<int> b) {
    b.insert(b.begin(), 0);
    b.insert(b.begin(), 0); // add two leading zeros
    vector<int> a = findDiffsArray(findDiffsArray(b));
    return a;
}

// [l, r) += [step, 2 * step, ..., (r - l) * step]
void addArithmOnHalfInterval(int l, int r, int step) {
    a[l] += step;
    if (r < n) {
        a[r] -= (r - l + 1) * step;
    }
    if (r + 1 < n) {
        a[r + 1] += (r - l) * step;
    }
}

vector<int> postcalc(const vector<int>& a) {
    vector<int> finalB = findPrefixSums(findPrefixSums(a));
    finalB.erase(finalB.begin());
    finalB.erase(finalB.begin()); // delete leading zeros
    return finalB;
}
\end{code}
