\begin{code}
const long long INF = 1e18;
vector<long long> tree;
vector<long long> push;
int n;
int logn;

void build(const vector<int>& arr) {
    n = arr.size();
    logn = 32 - __builtin_clz(2 * n);
    tree.assign(2 * n, 0);
    push.assign(n, 0);
    for (int i = 0; i < n; i++) {
        tree[n + i] = arr[i];
    }
    for (int i = n - 1; i > 0; i--) {
        tree[i] = max(tree[i << 1], tree[(i << 1) | 1]);
    }
}

void updateVertex(int v, long long val) {
    tree[v] += val;
    if (v < n) {
        push[v] += val;
    }
}

void updateAncestors(int v) {
    v >>= 1;
    while (v > 0) {
        tree[v] = max(tree[v << 1], tree[(v << 1) | 1]) + push[v];
        v >>= 1;
    }
}

void doPush(int leaf) {
    for (int k = logn; k > 0; k--) {
        int curV = (leaf >> k);
        updateVertex(curV << 1, push[curV]);
        updateVertex((curV << 1) | 1, push[curV]);
        push[curV] = 0;
    }
}

void updateSegment(int l, int r, int val) { // [l, r) += val
    l += n;
    r += n;
    int ql = l, qr = r;
    while (l < r) {
        if (l & 1) {
            updateVertex(l++, val);
        }
        if (r & 1) {
            updateVertex(--r, val);
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

