#include <bits/stdc++.h>

using namespace std;

struct BitRMQ {
    vector<int> arr;
    vector<vector<int>> sparse;
    vector<int> precalc;
    int n;
    int k;
    int blocks;
    int lg;
    const int INF = 1e9 + 7;

    BitRMQ(const vector<int>& inputArray) {
        arr = inputArray;
        n = arr.size();
        k = 32;
        blocks = (n + k - 1) / k;
        lg = ceil(log2(blocks + 1));
        sparse.assign(lg, vector<int>(blocks, 0));
        precalc.assign(n + 1, 0);
        buildSparse();
        buildPrecalc();
    }

    void buildSparse() {
        for (int block = 0; block < blocks; block++) {
            int l = block * k;
            int r = min(l + k, n);
            int blockMin = arr[l];
            for (int i = l + 1; i < r; i++) {
                blockMin = min(blockMin, arr[i]);
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
        int ms = 0;
        stack<int> minimaStack;
        for (int i = 0; i < n; i++) {
            precalc[i] = ms;
            ms <<= 1;
            ms |= 1;
            while (!minimaStack.empty() && arr[minimaStack.top()] > arr[i]) {
                int len = i - minimaStack.top();
                if (len < 32) {
                    ms ^= (1 << len);
                }
                minimaStack.pop();
            }
            minimaStack.push(i);
        }
        precalc[n] = ms;
    }
    
    int findShort(int left, int right) { // left and right are in one block
        if (left >= right) {
            return INF;
        }
        int len = right - left;
        int maxBit = 32 - __builtin_clz(precalc[right] & ((1LL << len) - 1));
        int ans = arr[right - maxBit];
        return ans;
    }

    int findLong(int lblock, int rblock) {
        if (lblock == rblock) {
            return INF;
        }
        int bit = 31 - __builtin_clz(rblock - lblock);
        int ans = min(sparse[bit][lblock], sparse[bit][rblock - (1 << bit)]);
        return ans;
    }

    int findMin(int left, int right) { // [left; right)
        int lblock = left / k;
        int rblock = right / k;
        if (lblock == rblock) {
            return findShort(left, right);
        } else {
            return min(findLong(lblock + 1, rblock), min(findShort(left, (lblock + 1) * k), findShort(rblock * k, right)));
        }
    }
};

int main() {
    int n;
    cin >> n;
    vector<int> arr(n);
    for (int& elem : arr) {
        cin >> elem;
    }
    BitRMQ rmq(arr);
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
