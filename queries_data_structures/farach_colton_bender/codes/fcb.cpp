#include <bits/stdc++.h>

using namespace std;

template<typename T>
struct RMQPM1 {
    vector<T> arr;
    vector<vector<T>> sparse;
    vector<vector<vector<int>>> precalc;
    vector<int> blockType;
    vector<int> logs;
    int n;
    int k;
    int blocks;
    int lg;
    T INF;
    function<int(T)> getKey;

    RMQPM1() {};

    RMQPM1(const vector<T>& arr, const T& infty, function<int(T)> getKey) : arr(arr), getKey(getKey) {
        n = arr.size();
        k = ceil(log2(n + 1) / 2);
        blocks = (n + k - 1) / k;
        lg = ceil(log2(blocks + 1));
        sparse.assign(lg, vector<T>(blocks));
        precalc.assign((1 << (k - 1)), vector<vector<int>>(k, vector<int>(k + 1, 0)));
        blockType.assign(blocks, 0);
        logs.assign(blocks, 0);
        INF = infty;

        buildLogs();
        buildSparse();
        buildPrecalc();
    }

    void buildLogs() {
        for (int i = 2; i < blocks; i++) {
            logs[i] = logs[i >> 1] + 1;
        }
    }

    void buildSparse() {
        for (int block = 0; block < blocks; block++) {
            int l = block * k;
            int r = min(l + k, n);
            T blockMin = arr[l];
            for (int i = l + 1; i < r; i++) {
                blockMin = min(blockMin, arr[i]);
                if (getKey(arr[i]) == getKey(arr[i - 1]) + 1) {
                    blockType[block] |= (1 << (i - l - 1));
                }
            }
            sparse[0][block] = blockMin;
        }

        for (int bit = 1; bit < lg; bit++) {
            for (int i = 0; i + (1 << bit) <= blocks; i++) {
                sparse[bit][i] = min(sparse[bit - 1][i], sparse[bit - 1][i + (1 << (bit - 1))]);
            }
        }
    }

    void buildPrecalc() {
        for (int mask = 0; mask < (1 << (k - 1)); mask++) {
            for (int l = 0; l < k; l++) {
                precalc[mask][l][l] = -1;
                int curmin = numeric_limits<int>::max();
                int curval = 0;
                for (int r = l; r < k; r++) {
                    if (curval < curmin) {
                        precalc[mask][l][r + 1] = r;
                        curmin = curval;
                    } else {
                        precalc[mask][l][r + 1] = precalc[mask][l][r];
                    }
                    if ((mask >> r) & 1) {
                        curval++;
                    } else {
                        curval--;
                    }
                }
            }
        }
    }
    
    T findShort(int left, int right) { // left and right are in one block
        if (left == right) {
            return INF;
        }
        int block = left / k;
        T ans = arr[block * k + precalc[blockType[block]][left - block * k][right - block * k]];
        return ans;
    }

    T findLong(int lblock, int rblock) {
        if (lblock == rblock) {
            return INF;
        }
        int bit = logs[rblock - lblock];
        T ans = min(sparse[bit][lblock], sparse[bit][rblock - (1 << bit)]);
        return ans;
    }

    T findMin(int left, int right) { // [left; right)
        int lblock = left / k;
        int rblock = right / k;
        if (lblock == rblock) {
            return findShort(left, right);
        } else {
            return min(findLong(lblock + 1, rblock), min(findShort(left, (lblock + 1) * k), findShort(rblock * k, right)));
        }
    }
};

struct FCBLCA {
    vector<vector<int>> graph;
    vector<pair<int, int>> eulertour;
    vector<int> timein;
    int n;
    RMQPM1<pair<int, int>> rmq;

    FCBLCA(const pair<vector<vector<int>>, int>& graphandroot) : graph(graphandroot.first) {
        n = graph.size();
        timein.assign(n, 0);
        dfs(graphandroot.second);
        rmq = RMQPM1<pair<int, int>>(eulertour, {1e9 + 7, 0}, [](pair<int, int> pr) { return pr.first; });
    }

    void dfs(int v, int depth = 0, int anc = -1) {
        timein[v] = eulertour.size();
        eulertour.emplace_back(depth, v);
        for (int u : graph[v]) {
            if (u != anc) {
                dfs(u, depth + 1, v);
                eulertour.emplace_back(depth, v);
            }
        }
    }

    int findLCA(int u, int v) {
        if (timein[u] > timein[v]) {
            swap(u, v);
        }
        return rmq.findMin(timein[u], timein[v] + 1).second;
    }
};

struct FCBRMQ {
    vector<int> inputArray;
    FCBLCA fcblca;

    FCBRMQ(const vector<int>& arr) : inputArray(arr), fcblca(buildTreap(arr)) {}

    pair<vector<vector<int>>, int> buildTreap(const vector<int>& arr) {
        int n = arr.size();
        vector<int> left(n, -1);
        vector<int> right(n, -1);
        stack<int> rightBranch;
        rightBranch.push(0);
        int root = 0;
        for (int i = 1; i < n; i++) {
            int lastVertex = -1;
            while (!rightBranch.empty() && arr[rightBranch.top()] >= arr[i]) {
                lastVertex = rightBranch.top();
                rightBranch.pop();
            }
            left[i] = lastVertex;
            if (!rightBranch.empty()) {
                right[rightBranch.top()] = i;
            }
            if (rightBranch.empty()) {
                root = i;
            }
            rightBranch.push(i);
        }
        vector<vector<int>> graph(n);
        for (int i = 0; i < n; i++) {
            if (left[i] != -1) {
                graph[i].emplace_back(left[i]);
            }
            if (right[i] != -1) {
                graph[i].emplace_back(right[i]);
            }
        }
        return {graph, root};
    }

    int findMin(int l, int r) {
        return inputArray[fcblca.findLCA(l, r - 1)];
    }
};

int main() {
    int n;
    cin >> n;
    vector<int> arr(n);
    for (int& elem : arr) {
        cin >> elem;
    }
    FCBRMQ rmq(arr);
    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int left, right;
        cin >> left >> right;
        left--; // 0-indexing and half-intervals
        cout << rmq.findMin(left, right) << '\n';
    }
    return 0;
}
