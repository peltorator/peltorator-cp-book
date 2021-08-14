#include <bits/stdc++.h>

using namespace std;

const long long INF = 1e18 + 7;

struct Line {
    long long k, b;

    Line(long long k, long long b): k(k), b(b) {}

    Line(): k(0), b(INF) {}

    long long operator()(long long x) {
        return k * x + b;
    }
};

struct LiChaoSegmentTree {
    static const int T = (1 << 23);

    Line tree[T];

    int n;

    void init(int len) {
        n = len;
    }

    void insertLine(int v, int l, int r, Line newLine) {
        bool dominateLeft = (newLine(l) < tree[v](l));
        int mid = (r + l) / 2;
        bool dominateMid = (newLine(mid) < tree[v](mid));
        if (dominateMid) {
            swap(newLine, tree[v]);
        }
        if (l + 1 == r) {
            return;
        } else if (dominateLeft == dominateMid) {
            insertLine(2 * v + 1, mid, r, newLine);
        } else {
            insertLine(2 * v, l, mid, newLine);
        }
    }

    void insertLine(Line newLine) {
        insertLine(1, 0, n, newLine);
    }

    long long getValue(int v, int l, int r, int pos) {
        long long ans = tree[v](pos);
        if (l + 1 != r) {
            int mid = (r + l) / 2;
            if (pos < mid) {
                ans = min(ans, getValue(2 * v, l, mid, pos));
            } else {
                ans = min(ans, getValue(2 * v + 1, mid, r, pos));
            }
        }
        return ans;
    }

    long long getValue(int pos) {
        return getValue(1, 0, n, pos);
    }

} segTree;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;
    segTree.init(n);

    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int type;
        cin >> type;
        if (type == 1) {
            int k, b;
            cin >> k >> b;
            segTree.insertLine(Line(k, b));
        } else {
            int x;
            cin >> x;
            cout << segTree.getValue(x) << '\n';
        }
    }
}
