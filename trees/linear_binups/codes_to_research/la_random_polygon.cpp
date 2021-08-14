#include <bits/stdc++.h>

using namespace std;

mt19937 rnd(1234);

struct LinearBinups {
    vector<int> depth;
    vector<int> parent;
    vector<int> jump;

    void init(int n, int root = 0) {
        depth.assign(n, 0);
        parent.assign(n, 0);
        jump.assign(n, 0);
        parent[root] = jump[root] = root;
    }

    void addLeaf(int v, int par) {
        parent[v] = par;
        depth[v] = depth[par] + 1;
        int jumpDepth = rnd() % depth[v];
        jump[v] = findLA(par, jumpDepth);
    }

    int findLA(int v, int k) {
        int h = max(depth[v] - k, 0);
        while (depth[v] != h) {
            if (depth[jump[v]] >= h) {
                v = jump[v];
            } else {
                v = parent[v];
            }
        }
        return v;
    }
} linearBinups;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, q;
    cin >> n >> q;
    linearBinups.init(n);

    vector<vector<int>> g(n);

    for (int i = 1; i < n; i++) {
        int par;
        cin >> par;
        g[par].push_back(i);
    }

    std::function<void(int, int)> dfs = [&](int v, int par) {
        if (v != 0) {
            linearBinups.addLeaf(v, par);
        }
        for (int u : g[v]) {
            dfs(u, v);
        }
    };
    dfs(0, 0);

    int a1, a2;
    long long x, y, z;
    cin >> a1 >> a2 >> x >> y >> z;
    long long sum = 0;
    int prev = 0;
    for (int i = 0; i < q; i++) {
        int q1 = (a1 + prev) % n;
        int q2 = a2;
        int ans = linearBinups.findLA(q1, q2);
        int a3 = (x * a1 + y * a2 + z) % n;
        int a4 = (x * a2 + y * a3 + z) % n;
        a1 = a3;
        a2 = a4;
        sum += ans;
        prev = ans;
    }
    cout << sum << endl;
}
