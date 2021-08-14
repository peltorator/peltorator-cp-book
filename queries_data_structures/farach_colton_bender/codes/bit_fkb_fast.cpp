#include <bits/stdc++.h>

using namespace std;

namespace BitRMQ {
    static const int K = 32;
    static const int N = 20000064;
    static const int BLOCKS = 625002;
    static const int LOG = 21;
    static const int INF = 1e9 + 7;

    int arr[N];
    int sparse[LOG][BLOCKS];
    int precalc[N];
    int n;

    void buildSparse() {
        for (int block = 0; block < BLOCKS; block++) {
            int l = block * K;
            int r = min(l + K, n);
            int blockMin = arr[l];
            for (int i = l + 1; i < r; i++) {
                blockMin = min(blockMin, arr[i]);
            }
            sparse[0][block] = blockMin;
        }

        for (int bit = 1; bit < LOG; bit++) {
            for (int i = 0; i + (1 << bit) <= BLOCKS; i++) {
                sparse[bit][i] = min(sparse[bit - 1][i], sparse[bit - 1][i + (1 << (bit - 1))]);
            }
        }
    }

    void buildPrecalc() {
        int ms = 0;
        for (int i = 0; i < n; i++) {
            precalc[i] = ms;
            ms <<= 1;
            while (ms != 0) {
                int bt = __builtin_ctz(ms);
                if (arr[i - bt] < arr[i]) {
                    break;
                }
                ms ^= (1 << bt);
            }
            ms |= 1;
        }
        precalc[n] = ms;
    }

    void init(int arrayLen) {
        n = arrayLen;
        buildSparse();
        buildPrecalc();
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
        int lblock = left / K;
        int rblock = right / K;
        if (lblock == rblock) {
            return findShort(left, right);
        } else {
            return min(findLong(lblock + 1, rblock), min(findShort(left, (lblock + 1) * K), findShort(rblock * K, right)));
        }
    }
}

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> BitRMQ::arr[i];
    }
    BitRMQ::init(n);
    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int left, right;
        cin >> left >> right;
        left--; // 0-indexing and half-intervals
        cout << BitRMQ::findMin(left, right) << '\n';
    }
    return 0;
}
