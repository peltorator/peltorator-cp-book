\begin{code}
vector<int> precalc(vector<int> b) {
    b.insert(b.begin(), 0); // add leading zero
    vector<int> a = findDiffsArray(b);
    return a;
}

void addOnHalfInterval(int l, int r, int d) { // [l, r) += d
    a[l] += d;
    if (r < n) {
        a[r] -= d;
    }
}

vector<int> postcalc(const vector<int>& a) {
    vector<int> finalB = findPrefixSums(a);
    finalB.erase(finalB.begin()); // delete leading zero
    return finalB;
}
\end{code}
