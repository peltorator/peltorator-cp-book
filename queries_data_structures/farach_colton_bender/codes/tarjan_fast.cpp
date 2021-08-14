#include <bits/stdc++.h>

using namespace std;

const int N = 1e6, M = N;

int arr[N];
int ans[N];
int minimaStack[N];

int ancestor[N];
int setSize[N];
int setMinimum[N];

int l[M], r[M];
int sortedQueriesIds[M];
int indexCounts[N + 1];

void initDSU(int n) {
    for (int i = 0; i < n; i++) {
        ancestor[i] = i;
        setSize[i] = 1;
        setMinimum[i] = arr[i];
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

void solveRMQ(int n, int q) {
    for (int i = 0; i < q; i++) {
        indexCounts[r[i] + 1]++;
    }
    for (int i = 0; i < N; i++) {
        indexCounts[i + 1] += indexCounts[i];
    }
    for (int i = 0; i < q; i++) {
        sortedQueriesIds[indexCounts[r[i]]++] = i;
    }
    initDSU(n);
    int stackSize = 0;
    int sortedQueriesIter = 0;
    for (int i = 0; i < n; i++) {
        while (stackSize > 0 && arr[i] <= arr[minimaStack[stackSize - 1]]) {
            unionSets(i, minimaStack[stackSize - 1]);
            stackSize--;
        }
        minimaStack[stackSize++] = i;
        while (sortedQueriesIter < q && r[sortedQueriesIds[sortedQueriesIter]] == i) {
            int curid = sortedQueriesIds[sortedQueriesIter++];
            ans[curid] = findSetMinimum(l[curid]);
        }
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }
    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        cin >> l[i] >> r[i];
        l[i]--;
        r[i]--; // 0-indexing
    }
    solveRMQ(n, q);
    for (int i = 0; i < q; i++) {
        cout << ans[i] << '\n';
    }
    return 0;
}
