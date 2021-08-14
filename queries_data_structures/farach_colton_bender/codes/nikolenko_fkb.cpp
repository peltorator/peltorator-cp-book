#pragma GCC target("sse4")
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

using namespace std;

const int N = 20000004; // N % K = 0
const int K = 12;
const int BLOCKS = (N / K);
const int LOG = (int) ceil(log2(BLOCKS));
const int Q = 22;

int a[N];
int type[BLOCKS];
int sp[Q][BLOCKS];

void maxi(int &x, const int y) {
    if (y > x) {
        x = y;
    }
}

void build(int level, int l, int r) {
    if (r - l <= 1) {
        return;
    }
    int mid = (l + r) / 2;
    if (mid - 1 < BLOCKS) {
        int cur_max = numeric_limits<int>::min();
        for (int i = mid - 1; i >= l; i--) {
            maxi(cur_max, sp[0][i]);
            sp[level][i] = cur_max;
        }
    }
    if (mid < BLOCKS) {
        int cur_max = numeric_limits<int>::min();
        int tmp_r = min(BLOCKS, r);
        for (int i = mid; i < tmp_r; i++) {
            maxi(cur_max, sp[0][i]);
            sp[level][i] = cur_max;
        }
    }

    build(level - 1, l, mid);
    build(level - 1, mid, r);
}

int get(int l, int r) {
    if (l > r) {
        return numeric_limits<int>::min();
    }
    if (l == r) {
        return sp[0][l];
    }
    
    int bit = 32 - __builtin_clz(l ^ r);
    return max(sp[bit][l], sp[bit][r]);
}

bool test(int mask, int bit) {
    return mask & (1 << bit);
}

int precalc[1 << (K - 1)][K][K];

int query(int l, int r) {
    int l_block = l / K;
    int r_block = r / K;
    if (l_block == r_block) {
        int result = precalc[type[l_block]][l - l_block * K][r - r_block * K];
        result += a[l];
        return result;
    }

    int left = precalc[type[l_block]][l - l_block * K][K - 1];
    left += a[l];
    int right = precalc[type[r_block]][0][r - r_block * K];
    right += a[r_block * K];
    int mid = get(l_block + 1, r_block - 1);
    maxi(left, right);
    maxi(left, mid);
    return left;
}

const long long MOD = 1LL << 31;

void tt() {
    cerr << "curtime: " << clock() * 1.0 / CLOCKS_PER_SEC << endl;
}

int main() {
    int n, b, c, d;
    cin >> n >> a[0] >> b >> c >> d;
    for (int i = 1; i < n; i++) {
        b = (int) ((b * 1LL * c + d) % MOD);
        a[i] = a[i - 1] + (1 - 2 * ((b % 239179) & 1));
    }

    for (int block = 0; block < BLOCKS; block++) {
        int l = block * K;
        int r = l + K;
        int cur_max = a[l];
        for (int i = l + 1; i < r; i++) {
            if (a[i] == a[i - 1] + 1) {
                type[block] ^= (1 << (i - l - 1));
            }
            maxi(cur_max, a[i]);
        }
        sp[0][block] = cur_max;
    }

    build(LOG, 0, 1 << LOG); 

    for (int mask = 0; mask < (1 << (K - 1)); mask++) {
        for (int l = 0; l < K; l++) {
            int cur_val = 0;
            int cur_max = 0;
            for (int r = l; r < K; r++) {
                maxi(cur_max, cur_val);
                precalc[mask][l][r] = cur_max;
                if (test(mask, r)) {
                    cur_val++;
                } else {
                    cur_val--;
                }
            }
        }
    }

    long long x;
    cin >> x;
    long long total = 0;
    int ans = 0;
    tt();
    for (int i = 0; i < n; i++) {
        int l = (int) ((x + ans) % n);
        if (l < 0) {
            l += n;
        }
        int r = (int) ((l + i) % n);
        ans = query(min(l, r), max(l, r));
        total += ans;
        x = ((x * 1103515245LL + 12345) % MOD);
    }

    cout << total << "\n";
    tt();
    return 0;
}
