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

void update_vertex(int v, long long val) {
    tree[v] += val;
    if (v < n) {
        push[v] += val;
    }
}

void update_ancestors(int v) {
    v >>= 1;
    while (v > 0) {
        tree[v] = max(tree[v << 1], tree[(v << 1) | 1]) + push[v];
        v >>= 1;
    }
}

void do_push(int leaf) {
    for (int k = logn; k > 0; k--) {
        int v = (leaf >> k);
        update_vertex(v << 1, push[v]);
        update_vertex((v << 1) | 1, push[v]);
        push[v] = 0;
    }
}

void update_segment(int l, int r, int val) { // [l, r) += val
    l += n;
    r += n;
    int ql = l, qr = r;
    while (l < r) {
        if (l & 1) {
            update_vertex(l++, val);
        }
        if (r & 1) {
            update_vertex(--r, val);
        }
        l >>= 1;
        r >>= 1;
    }
    update_ancestors(ql);
    update_ancestors(qr - 1);
}

long long find_max(int l, int r) { // [l, r)
    l += n;
    r += n;
    do_push(l);
    do_push(r - 1);
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

