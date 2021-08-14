#include <bits/stdc++.h>

using namespace std;

struct RMQPM1 {
    vector<int> arr;
    vector<vector<int>> sparse;
    vector<vector<vector<int>>> precalc;
    vector<int> blockType;
    vector<int> logs;
    int n;
    int k;
    int blocks;
    int lg;
    const int INF = 1e9 + 7;


    RMQPM1(const vector<int>& inputArray) {
        arr = inputArray;
        n = arr.size();
        k = ceil(log2(n + 1) / 2);
        blocks = (n + k - 1) / k;
        lg = ceil(log2(blocks + 1));
        sparse.assign(lg, vector<int>(blocks, 0));
        precalc.assign((1 << (k - 1)), vector<vector<int>>(k, vector<int>(k + 1, 0)));
        blockType.assign(blocks, 0);
        logs.assign(blocks, 0);
        
        buildLogs();
        buildSparse();
        buildPrecalc();
    }

    void buildLogs() {
        logs[1] = 0;
        for (int i = 2; i < blocks; i++) {
            logs[i] = logs[i >> 1] + 1;
        }
    }

    void buildSparse() {
        for (int block = 0; block < blocks; block++) {
            int l = block * k;
            int r = min(l + k, n);
            int blockMin = arr[l];
            for (int i = l + 1; i < r; i++) {
                blockMin = min(blockMin, arr[i]);
                if (arr[i] == arr[i - 1] + 1) {
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
                precalc[mask][l][l] = INF;
                int curval = 0;
                for (int r = l; r < k; r++) {
                    precalc[mask][l][r + 1] = min(precalc[mask][l][r], curval);
                    if ((mask >> r) & 1) {
                        curval++;
                    } else {
                        curval--;
                    }
                }
            }
        }
    }
    
    int findShort(int left, int right) { // left and right are in one block
        int block = left / k;
        int ans = precalc[blockType[block]][left - block * k][right - block * k] + arr[left];
        return ans;
    }

    int findLong(int lblock, int rblock) {
        if (lblock == rblock) {
            return INF;
        }
        int bit = logs[rblock - lblock];
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
    RMQPM1 rmq(arr);
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
