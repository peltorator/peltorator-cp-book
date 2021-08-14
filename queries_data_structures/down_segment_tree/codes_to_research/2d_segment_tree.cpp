#include <bits/stdc++.h>

using namespace std;

struct SegmentTree2D {
    static const int N = 1000, M = 1000;
    int n, m;
    long long tree[2 * N][2 * M];

    void build(const vector<vector<int>>& arr) {
        n = arr.size();
        m = arr[0].size();
        for (int i = 2 * n - 1; i > 0; i--) {
            for (int j = 2 * m - 1; j > 0; j--) {
                if (i < n) {
                    tree[i][j] = tree[2 * i][j] + tree[2 * i + 1][j];
                } else if (j < m) {
                    tree[i][j] = tree[i][2 * j] + tree[i][2 * j + 1];
                } else {
                    tree[i][j] = arr[i - n][j - m];
                }
            }
        }
    }

    void updatePoint(int x, int y, int newVal) {
        x += n;
        y += m;
        int curx = x;
        while (curx > 0) {
            int cury = y;
            while (cury > 0) {
                if (curx < n) {
                    tree[curx][cury] = tree[2 * curx][cury] + tree[2 * curx + 1][cury];
                } else if (cury < m) {
                    tree[curx][cury] = tree[curx][2 * cury] + tree[curx][2 * cury + 1];
                } else {
                    tree[curx][cury] = newVal;
                }
                cury >>= 1;
            }
            curx >>= 1;
        }
    }

    long long findSum(int lx, int rx, int ly, int ry) { // [lx, rx) * [ly, ry)
        lx += n;
        rx += n;

        long long ans = 0;
        while (lx < rx) {
            int curly = ly + m;
            int curry = ry + m;
            while (curly < curry) {
                if (curly & 1) {
                    if (lx & 1) {
                        ans += tree[lx][curly];
                    }
                    if (rx & 1) {
                        ans += tree[rx - 1][curly];
                    }
                }
                if (curry & 1) {
                    if (lx & 1) {
                        ans += tree[lx][curry - 1];
                    }
                    if (rx & 1) {
                        ans += tree[rx - 1][curry - 1];
                    }
                }
                curly = (curly + 1) >> 1;
                curry >>= 1;
            }
            lx = (lx + 1) >> 1;
            rx >>= 1;
        }
        return ans;
    }
} segTree;

int main() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> arr(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> arr[i][j];
        }
    }
    segTree.build(arr);

    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int type;
        cin >> type;
        if (type == 1) {
            int x, y, newVal;
            cin >> x >> y >> newVal;
            segTree.updatePoint(x - 1, y - 1, newVal);
        } else {
            int lx, rx, ly, ry;
            cin >> lx >> rx >> ly >> ry;
            cout << segTree.findSum(lx - 1, rx, ly - 1, ry) << '\n';
        }
    }
}
