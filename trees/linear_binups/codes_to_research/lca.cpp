#include <bits/stdc++.h>

using namespace std;

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
        if (depth[par] - depth[jump[par]] == depth[jump[par]] - depth[jump[jump[par]]]) {
            jump[v] = jump[jump[par]];
        } else {
            jump[v] = par;
        }
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

    int findLCA(int v, int u) {
        if (depth[v] > depth[u]) {
            v = findLA(v, depth[v] - depth[u]);
        } else {
            u = findLCA(u, depth[u] - depth[v]);
        }
        while (v != u) {
            if (jump[v] != jump[u]) {
                v = jump[v];
                u = jump[u];
            } else {
                v = parent[v];
                u = parent[u];
            }
        }
        return v;
    }
} linearBinups;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;
    linearBinups.init(n);

    for (int i = 1; i < n; i++) {
        int par;
        cin >> par; // par < i
        linearBinups.addLeaf(i, par - 1);
    }

    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int v, u;
        cin >> v >> u;
        cout << linearBinups.findLCA(v - 1, u - 1) << '\n';
    }
}
