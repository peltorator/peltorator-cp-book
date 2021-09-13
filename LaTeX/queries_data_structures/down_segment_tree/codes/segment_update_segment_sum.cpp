\begin{code}
const int H = 20; // 2^H >= 2n
const int N = (1 << H); // N >= n
const long long INF = 1e18;
long long tree[2 * N];
long long push[N];
int n;

void build(const vector<int>& arr) {
    n = arr.size();
    for (int i = 0; i < n; i++) {
        tree[n + i] = arr[i];
    }
    for (int i = n - 1; i > 0; i--) {
        tree[i] = tree[i << 1] + tree[(i << 1) | 1];
        push[i] = 0;
    }
}

void apply(int v, int val) {
    tree[v] += val;
    if (v < n) {
        push[v] += val;
    }
}

void updateAncestors(int v) {
    v >>= 1;
    while (v > 0) {
        tree[v] = tree[v << 1] + tree[(v << 1) | 1];
        v >>= 1;
    }
}

void doPush(int leaf) {
    for (int k = H; k > 0; k--) {
        int curV = (leaf >> k);
        apply(curV << 1, push[curV]);
        apply((curV << 1) | 1, push[curV]);
        push[curV] = 0;
    }
}

void updateSegment(int l, int r, int val) { // [l, r) += val
    l += n;
    r += n;
    int ql = l, qr = r;
    doPush(ql);
    doPush(qr - 1);
    while (l < r) {
        if (l & 1) {
            apply(l++, val);
        }
        if (r & 1) {
            apply(r--, val);
        }
        l >>= 1;
        r >>= 1;
    }
    updateAncestors(ql);
    updateAncestors(qr - 1);
}

long long findMax(int l, int r) { // [l, r)
    l += n;
    r += n;
    doPush(l);
    doPush(r - 1);
    long long ans = -INF;
    while (l < r) {
        if (l & 1) {
            ans = max(ans, tree[l++]);
        }
        if (r & 1) {
            ans = max(ans, tree[--r]);
        }
        l >>= 1;
        r >>= 1;
    }
    return ans;
}
\end{code}

