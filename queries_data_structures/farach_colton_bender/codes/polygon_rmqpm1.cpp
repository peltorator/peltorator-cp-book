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
        int blockMax = arr[l];
        for (int i = l + 1; i < r; i++) {
            blockMax = max(blockMax, arr[i]);
            if (arr[i] == arr[i - 1] + 1) {
                blockType[block] |= (1 << (i - l - 1));
            }
        }
        sparse[0][block] = blockMax;
    }

    for (int bit = 1; bit < LG; bit++) {
        for (int i = 0; i + (1 << bit) <= BLOCKS; i++) {
            sparse[bit][i] = max(sparse[bit - 1][i], sparse[bit - 1][i + (1 << (bit - 1))]);
        }
    }
}

void buildPrecalc() {
    for (int mask = 0; mask < (1 << (K - 1)); mask++) {
        for (int l = 0; l < K; l++) {
            precalc[mask][l][l] = -INF;
            int curval = 0;
            for (int r = l; r < K; r++) {
                precalc[mask][l][r + 1] = max(precalc[mask][l][r], curval);
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
    if (left == right) {
        return -INF;
    }
    int block = left / K;
    int ans = precalc[blockType[block]][left - block * K][right - block * K] + arr[left];
    return ans;
}

int findLong(int lblock, int rblock) {
    if (lblock == rblock) {
        return -INF;
    }
    int bit = 31 - __builtin_clz(rblock - lblock);
    int ans = max(sparse[bit][lblock], sparse[bit][rblock - (1 << bit)]);
    return ans;
}

int findMax(int left, int right) { // [left; right)
    int lblock = left / K;
    int rblock = right / K;
    if (lblock == rblock) {
        return findShort(left, right);
    } else {
        return max(findLong(lblock + 1, rblock), max(findShort(left, (lblock + 1) * K), findShort(rblock * K, right)));
    }
}

const long long MOD = 1LL << 31;

int main() {
    int n, b, c, d;
    cin >> n >> arr[0] >> b >> c >> d;
    for (int i = 1; i < n; i++) {
        b = (int) ((b * 1LL * c + d) % MOD);
        arr[i] = arr[i - 1] + (1 - 2 * ((b % 239179) & 1));
    }

    buildSparse();
    buildPrecalc();

    long long x;
    cin >> x;
    long long total = 0;
    int ans = 0;
    for (int i = 0; i < n; i++) {
        int l = (int) ((x + ans) % n);
        if (l < 0) {
            l += n;
        }
        int r = (int) ((l + i) % n);
        ans = findMax(min(l, r), max(l, r) + 1);
        total += ans;
        x = ((x * 1103515245LL + 12345) % MOD);
    }

    cout << total << "\n";
    return 0;
}
