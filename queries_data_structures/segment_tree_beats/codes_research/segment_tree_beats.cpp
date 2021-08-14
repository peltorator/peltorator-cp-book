#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
    
const int N = 100000;

struct JiDriverSegmentTree {
    static const int T = (1 << 18);
    static const int INF = 1e9 + 7;

    struct Node {
        int max;
        int maxCnt;
        int secondMax;
        ll sum;

        Node():
            max(INF),
            maxCnt(1),
            secondMax(-1),
            sum(INF) {}
    } tree[T];

    void build(int v, int l, int r) {
        tree[v].max = INF;
        tree[v].secondMax = -1;
        tree[v].maxCnt = r - l;
        tree[v].sum = 1LL * tree[v].maxCnt * tree[v].max;
        if (l + 1 != r) {
            int mid = (r + l) / 2;
            build(2 * v, l, mid);
            build(2 * v + 1, mid, r);
        }
    }

    JiDriverSegmentTree() {
        build(1, 0, N);
    }

    void updateWithVal(int v, int val) {
        if (tree[v].max > val) {
            tree[v].sum -= 1LL * (tree[v].max - val) * tree[v].maxCnt;
            tree[v].max = val;
        }
    }

    void pushToChildren(int v) {
        updateWithVal(2 * v, tree[v].max);
        updateWithVal(2 * v + 1, tree[v].max);
    }

    void updateFromChildren(int v) {
        tree[v].sum = tree[2 * v].sum + tree[2 * v + 1].sum;
        tree[v].max = max(tree[2 * v].max, tree[2 * v + 1].max);
        tree[v].secondMax = max(tree[2 * v].secondMax, tree[2 * v + 1].secondMax);
        tree[v].maxCnt = 0;
        if (tree[2 * v].max == tree[v].max) {
            tree[v].maxCnt += tree[2 * v].maxCnt;
        } else {
            tree[v].secondMax = max(tree[v].secondMax, tree[2 * v].max);
        }
        if (tree[2 * v + 1].max == tree[v].max) {
            tree[v].maxCnt += tree[2 * v + 1].maxCnt;
        } else {
            tree[v].secondMax = max(tree[v].secondMax, tree[2 * v + 1].max);
        }
    }

    void update(int v, int l, int r, int ql, int qr, int val) {
        if (qr <= l || r <= ql || tree[v].max <= val) {
            return;
        }
        if (ql <= l && r <= qr && tree[v].secondMax < val) {
            updateWithVal(v, val);
            return;
        }
        pushToChildren(v);
        int mid = (r + l) / 2;
        update(2 * v, l, mid, ql, qr, val);
        update(2 * v + 1, mid, r, ql, qr, val);
        updateFromChildren(v);
    }

    long long find(int v, int l, int r, int ql, int qr) {
        if (qr <= l || r <= ql) {
            return 0;
        }
        if (ql <= l && r <= qr) {
            return tree[v].sum;
        }
        pushToChildren(v);
        int mid = (r + l) / 2;
        return find(2 * v, l, mid, ql, qr) + find(2 * v + 1, mid, r, ql, qr);
    }
} segTree[4];

int main() {
    freopen("out.txt", "w", stdout);
    int q;
    cin >> q;
    vector<set<int>> notUsed(2);
    for (int i = 0; i < N; i++) {
        notUsed[0].insert(i);
        notUsed[1].insert(i);
    }
    while (q--) {
        int type, ql, qr;
        cin >> type >> ql >> qr;
        ql--;
        qr--;
        if (type == 1) {
            int k;
            cin >> k;
            int ind = 0;
            if (k < 0) {
                ind = 1;
                k = abs(k);
            }
            segTree[ind].update(1, 0, N, ql, qr, k);
            segTree[ind + 2].update(1, 0, N, ql, qr, k);
            while (true) {
                auto firstIt = notUsed[ind].lower_bound(ql);
                if (firstIt == notUsed[ind].end() || *firstIt >= qr) {
                    break;
                }
                int pos = *firstIt;
                notUsed[ind].erase(firstIt);
                if (!notUsed[ind ^ 1].count(pos)) {
                    for (int j = 2; j < 4; j++) {
                        segTree[j].update(1, 0, N, pos, pos + 1, 0);
                    }
                }
            }
        } else {
            long long sum = 0;
            for (int i = 0; i < 4; i++) {
                sum += (i < 2 ? 1 : -1) * segTree[i].find(1, 0, N, ql, qr);
            }
            cout << sum << '\n';
        }
    }
    return 0;
}
