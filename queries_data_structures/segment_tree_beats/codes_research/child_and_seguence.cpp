#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

const int LG = 31;

int max_query_cnt = 0;

const int T = (1 << 20);

int a[T];
ll sum[T];
int mx[T];
int mn[T];
int psh[T];

void upd(int v) {
    sum[v] = sum[2 * v] + sum[2 * v + 1];
    mx[v] = max(mx[2 * v], mx[2 * v + 1]);
    mn[v] = min(mn[2 * v], mn[2 * v + 1]);
}

void build(int v, int l, int r) {
    psh[v] = -1;
    if (l + 1 == r) {
        sum[v] = mn[v] = mx[v] = a[l];
        return;
    }
    int mid = (r + l) / 2;
    build(2 * v, l, mid);
    build(2 * v + 1, mid, r);
    upd(v);
}

void push(int v, int l, int r) {
    if (psh[v] != -1) {
        int mid = (r + l) / 2;
        mn[2 * v] = mx[2 * v] = mn[2 * v + 1] = mx[2 * v + 1] = psh[v];
        sum[2 * v] = 1LL * (mid - l) * psh[v];
        sum[2 * v + 1] = 1LL * (r - mid) * psh[v];
        psh[2 * v] = psh[2 * v + 1] = psh[v];
        psh[v] = -1;
    }
}

void seg_mod(int v, int l, int r, int ql, int qr, int x) {
    max_query_cnt--;
    if (r <= ql || qr <= l || mx[v] < x) {
        return;
    }
    if (ql <= l && r <= qr && mn[v] == mx[v]) {
        mx[v] %= x;
        mn[v] %= x;
        sum[v] = 1LL * (r - l) * mx[v];
        psh[v] = mx[v];
        return;
    }
    push(v, l, r);
    int mid = (r + l) / 2;
    seg_mod(2 * v, l, mid, ql, qr, x);
    seg_mod(2 * v + 1, mid, r, ql, qr, x);
    upd(v);
}

void assign_val(int v, int l, int r, int q, int x) {
    if (l + 1 == r) {
        mx[v] = mn[v] = sum[v] = x;
        return;
    }
    push(v, l, r);
    int mid = (r + l) / 2;
    if (q < mid) {
        assign_val(2 * v, l, mid, q, x);
    } else {
        assign_val(2 * v + 1, mid, r, q, x);
    }
    upd(v);
}

ll find_sum(int v, int l, int r, int ql, int qr) {
    if (r <= ql || qr <= l) {
        return 0;
    }
    if (ql <= l && r <= qr) {
        return sum[v];
    }
    push(v, l, r);
    int mid = (r + l) / 2;
    return find_sum(2 * v, l, mid, ql, qr) + find_sum(2 * v + 1, mid, r, ql, qr);
}

int main() {
    int n, m;
    cin >> n >> m;
    max_query_cnt = (n + m) * LG * 2;
    for (int i = 0; i < n; i++) {
        cin >> a[i];
    }
    build(1, 0, n);
    for (int i = 0; i < m; i++) {
        int t;
        cin >> t;
        if (t == 1) {
            int l, r;
            cin >> l >> r;
            l--;
            cout << find_sum(1, 0, n, l, r) << '\n';
        } else if (t == 2) {
            int l, r, x;
            cin >> l >> r >> x;
            l--;
            seg_mod(1, 0, n, l, r, x);
        } else {
            int k, x;
            cin >> k >> x;
            k--;
            assign_val(1, 0, n, k, x);
        }
        if (max_query_cnt < 0) {
            while (true);
        }
    }
    return 0;
}
