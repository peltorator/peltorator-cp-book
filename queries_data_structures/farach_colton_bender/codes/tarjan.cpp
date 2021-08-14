#include <bits/stdc++.h>

using namespace std;

struct DSU {
    vector<int> ancestor;
    vector<int> setSize;
    vector<int> setMinimum;

    DSU(const vector<int>& arr) {
        ancestor.assign(arr.size(), 0);
        setSize.assign(arr.size(), 1);
        setMinimum = arr;
        for (size_t i = 0; i < arr.size(); i++) {
            ancestor[i] = i;
        }
    }

    int findRoot(int v) {
        if (ancestor[v] == v) {
            return v;
        } else {
            return ancestor[v] = findRoot(ancestor[v]);
        }
    }

    void unionSets(int u, int v) {
        u = findRoot(u);
        v = findRoot(v);
        if (setSize[u] < setSize[v]) {
            swap(u, v);
        }
        setSize[u] += setSize[v];
        ancestor[v] = u;
        setMinimum[u] = min(setMinimum[u], setMinimum[v]);
    }

    int findSetMinimum(int v) {
        return setMinimum[findRoot(v)];
    }
};

vector<int> solveRMQ(const vector<int>& arr, const vector<pair<int, int>>& queries) {
    int n = arr.size();
    int q = queries.size();
    vector<vector<pair<int, int>>> queriesForRightBounds(n);
    for (int i = 0; i < q; i++) {
        int left = queries[i].first, right = queries[i].second;
        queriesForRightBounds[right].emplace_back(left, i);
    }
    DSU dsu(arr);
    stack<int> minimaStack;
    vector<int> ans(q);
    for (int i = 0; i < n; i++) {
        while (!minimaStack.empty() && arr[i] <= arr[minimaStack.top()]) {
            dsu.unionSets(i, minimaStack.top());
            minimaStack.pop();
        }
        minimaStack.push(i);
        for (auto [left, qind] : queriesForRightBounds[i]) {
            ans[qind] = dsu.findSetMinimum(left);
        }
    }
    return ans;
}

int main() {
    int n;
    cin >> n;
    vector<int> arr(n);
    for (int& elem : arr) {
        cin >> elem;
    }
    int q;
    cin >> q;
    vector<pair<int, int>> queries(q);
    for (auto& [left, right] : queries) {
        cin >> left >> right;
        left--;
        right--; // 0-indexing
    }
    vector<int> ans = solveRMQ(arr, queries);
    for (int val : ans) {
        cout << val << '\n';
    }
    return 0;
}
