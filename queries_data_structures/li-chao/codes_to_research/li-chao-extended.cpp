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

    void add(Line other) {
        k += other.k;
        b += other.b;
    }
};

struct LiChaoSegmentTree {
    static const int T = (1 << 23);

    struct Node {
        Line curLine;
        Line pushLine = Line(0, 0);
    } tree[T];

    int n;

    void init(int len) {
        n = len;
    }

    void doPush(int v) {
        tree[2 * v].pushLine.add(tree[v].pushLine);
        tree[2 * v].curLine.add(tree[v].pushLine);

        tree[2 * v + 1].pushLine.add(tree[v].pushLine);
        tree[2 * v + 1].curLine.add(tree[v].pushLine);

        tree[v].pushLine = Line(0, 0);
    }

    void insertLine(int v, int l, int r, Line newLine) {
        bool dominateLeft = (newLine(l) < tree[v].curLine(l));
        int mid = (r + l) / 2;
        bool dominateMid = (newLine(mid) < tree[v].curLine(mid));
        if (dominateMid) {
            swap(newLine, tree[v].curLine);
        }
        if (l + 1 == r) {
            return;
        }
        doPush(v);
        if (dominateLeft == dominateMid) {
            insertLine(2 * v + 1, mid, r, newLine);
        } else {
            insertLine(2 * v, l, mid, newLine);
        }
    }
 
    void insertLineOnSegment(int v, int l, int r, int ql, int qr, Line newLine) {
        if (r <= ql || qr <= l) {
            return;
        }
        if (ql <= l && r <= qr) {
            insertLine(v, l, r, newLine);
            return;
        }
        doPush(v);
        int mid = (r + l) / 2;
        insertLineOnSegment(2 * v, l, mid, ql, qr, newLine);
        insertLineOnSegment(2 * v + 1, mid, r, ql, qr, newLine);
    }

    void insertLineOnSegment(int ql, int qr, Line newLine) {
        insertLineOnSegment(1, 0, n, ql, qr, newLine);
    }

    void freeVertex(int v, int l, int r) {
        int mid = (r + l) / 2;
        insertLine(2 * v, l, mid, tree[v].curLine);
        insertLine(2 * v + 1, mid, r, tree[v].curLine);
        tree[v].curLine = Line();
    }

    void addLineOnSegment(int v, int l, int r, int ql, int qr, Line newLine) {
        if (r <= ql || qr <= l) {
            return;
        }
        if (ql <= l && r <= qr) {
            tree[v].curLine.add(newLine);
            tree[v].pushLine.add(newLine);
            return;
        }
        doPush(v);
        freeVertex(v, l, r);
        int mid = (r + l) / 2;
        addLineOnSegment(2 * v, l, mid, ql, qr,  newLine);
        addLineOnSegment(2 * v + 1, mid, r, ql, qr,  newLine);
    }

    void addLineOnSegment(int ql, int qr, Line newLine) {
        addLineOnSegment(1, 0, n, ql, qr, newLine);
    }

    long long getValue(int v, int l, int r, int pos) {
        long long ans = tree[v].curLine(pos);
        if (l + 1 != r) {
            doPush(v);
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
            int l, r, k, b;
            cin >> l >> r >> k >> b;
            l--;
            segTree.insertLineOnSegment(l, r, Line(k, b));
        } else if (type == 2) {
            int l, r, k, b;
            cin >> l >> r >> k >> b;
            l--;
            segTree.addLineOnSegment(l, r, Line(k, b));
        } else {
            int x;
            cin >> x;
            cout << segTree.getValue(x) << '\n';
        }
    }
}
