#include <bits/stdc++.h>

using namespace std;

const int N = 20000004;
const int K = 12; // log2(N) / 2
const int BLOCKS = 1666667; // N / K
const int LG = 21; // log2(BLOCKS)
const int INF = 2e9 + 7;

int arr[N];
int sparse[LG][BLOCKS];
int precalc[1 << (K - 1)][K][K + 1];
int blockType[BLOCKS];

void buildSparse() {
    for (int block = 0; block < BLOCKS; block++) {
        int l = block * K;
        int r = min(l + K, N);
        int blockMin = arr[l];
        for (int i = l + 1; i < r; i++) {
            blockMin = min(blockMin, arr[i]);
            if (arr[i] == arr[i - 1] + 1) {
                blockType[block] |= (1 << (i - l - 1));
            }
        }
        sparse[0][block] = blockMin;
    }

    for (int bit = 1; bit < LG; bit++) {
        for (int i = 0; i + (1 << bit) <= BLOCKS; i++) {
            sparse[bit][i] = min(sparse[bit - 1][i], sparse[bit - 1][i + (1 << (bit - 1))]);
        }
    }
}

void buildPrecalc() {
    for (int mask = 0; mask < (1 << (K - 1)); mask++) {
        for (int l = 0; l < K; l++) {
            precalc[mask][l][l] = INF;
            int curval = 0;
            for (int r = l; r < K; r++) {
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
    int block = left / K;
    int ans = precalc[blockType[block]][left - block * K][right - block * K] + arr[left];
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

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    buildSparse();
    buildPrecalc();

    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int left, right;
        cin >> left >> right;
        left--; // 0-indexing and half-intervals
        cout << findMin(left, right) << '\n';
    }
    return 0;
}
