\begin{code}
const int N = (1 << 20); // N = 2^k, N >= n
long long tree[2 * N];

void build(const vector<int>& arr) {
    for (size_t i = 0; i < arr.size(); i++) {
        tree[N + i] = arr[i];
    }
    for (int i = N - 1; i > 0; i--) {
        tree[i] = tree[i << 1] + tree[(i << 1) | 1];
    }
}

void updatePoint(int pos, int newVal) {
    pos += N;
    tree[pos] = newVal;
    pos >>= 1;
    while (pos > 0) {
        tree[pos] = tree[pos << 1] + tree[(pos << 1) | 1];
        pos >>= 1;
    }
}

long long findSum(int l, int r) { // [l, r)
    l += N;
    r += N;
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
