\begin{code}
vector<long long> tree;
int n;

void build(const vector<int>& arr) {
    n = arr.size();
    tree.assign(2 * n, 0);
    for (int i = 0; i < n; i++) {
        tree[n + i] = arr[i];
    }
    // tree[0..n-1] are zeros because there's nothing to add on a segment
}

long long findValue(int pos) {
    pos += n;
    long long ans = 0;
    while (pos > 0) {
        ans += tree[pos];
        pos >>= 1;
    }
    return ans;
}

void segmentUpdate(int l, int r, int addVal) { // [l, r)
    l += n;
    r += n;
    while (l < r) {
        if (l & 1) {
            tree[l++] += addVal;
        }
        if (r & 1) {
            tree[--r] += addVal;
        }
        l >>= 1;
        r >>= 1;
    }
}
\end{code}
