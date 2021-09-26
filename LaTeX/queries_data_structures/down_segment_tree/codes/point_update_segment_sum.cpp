\begin{code}
vector<long long> tree;
int n;

void build(const vector<int>& arr) {
    n = arr.size();
    tree.assign(2 * n, 0);
    for (int i = 0; i < n; i++) {
        tree[n + i] = arr[i];
    }
    for (int i = n - 1; i > 0; i--) {
        tree[i] = tree[i << 1] + tree[(i << 1) | 1];
    }
}

void updatePoint(int pos, int newVal) {
    pos += n;
    tree[pos] = newVal;
    pos >>= 1;
    while (pos > 0) {
        tree[pos] = tree[pos << 1] + tree[(pos << 1) | 1];
        pos >>= 1;
    }
}

long long findSum(int l, int r) { // [l, r)
    l += n;
    r += n;
    long long ans = 0;
    while (l < r) {
        if (l & 1) {
            ans += tree[l++];
        }
        if (r & 1) {
            ans += tree[--r];
        }
        l >>= 1;
        r >>= 1;
    }
    return ans;
}
\end{code}
