#pragma GCC target("sse4")
#include <bits/stdc++.h>

using namespace std;

const int K = 32;
const int N = 20000064;
const int BLOCKS = N / K;
const int LOG = 21;
const int Q = 22;

int a[N];
int sp[Q][BLOCKS];
int keks;

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

int msb2(unsigned x) {
    union { double a; unsigned b[2]; };
    a = x;
    return (b[1] >> 20) - 1022;
}

int msb(unsigned i) {
    i |= (i >>  1);
    i |= (i >>  2);
    i |= (i >>  4);
    i |= (i >>  8);
    i |= (i >> 16);
    return i - (i >> 1);
}

int get(int l, int r) {
    if (l > r) {
        return numeric_limits<int>::min();
    }
    if (l == r) {
        return sp[0][l];
    }
    
    int bit = msb(l ^ r);//32 - __builtin_clz(l ^ r);
    return max(sp[bit][l], sp[bit][r]);
}

bool test(int mask, int bit) {
    return mask & (1 << bit);
}

int precalc[N];

inline int ask_small(int l, int r) {
    return l >= r ? numeric_limits<int>::min() : a[r - msb(precalc[r] & ((1LL << (r - l)) - 1))];
    //return l >= r ? numeric_limits<int>::min() : a[r - 32 + (__builtin_clz(precalc[r] & ((1LL << (r - l)) - 1)))];
    //return l >= r ? numeric_limits<int>::min() : a[r - 32] + (precalc[r] & ((1LL << (r - l)) - 1));
}

int query(int l, int r) {
    int l_block = l / K;
    int r_block = r / K;
    if (l_block == r_block) {
        return ask_small(l, r + 1);
    }

    int left = ask_small(l, (l_block + 1) * K);
    int right = ask_small(r_block * K, r + 1);
    int mid = get(l_block + 1, r_block - 1);
    maxi(left, right);
    maxi(left, mid);
    return left;
}

void calculate_precalc(int n) {
    int ms = 0;
    for (int i = 0; i < n; i++) {
        precalc[i] = ms;
        ms <<= 1;
        while (ms != 0) {
            int bt = __builtin_ctz(ms);
            if (a[i - bt] > a[i]) {
                break;
            }
            ms ^= (1 << bt);
        }
        ms |= 1;
    }
    precalc[n] = ms;
}

const long long MOD = 1LL << 31;

void tt() {
    cerr << "curtime: " << clock() * 1.0 / CLOCKS_PER_SEC << endl;
}

int main() {
    for (int i = 1; i <= 5; i++) {
        cout << msb(i) << ' ' << msb2(i) << endl;
    }
    return 0;
    tt();
    int n, b, c, d;
    cin >> n >> a[0] >> b >> c >> d;
    keks = n;
    for (int i = 1; i < n; i++) {
        b = (int) ((b * 1LL * c + d) % MOD);
        a[i] = a[i - 1] + (1 - 2 * ((b % 239179) & 1));
    }
    tt();
    for (int block = 0; block < BLOCKS; block++) {
        int l = block * K;
        int r = l + K;
        int cur_max = a[l];
        for (int i = l + 1; i < r; i++) {
            maxi(cur_max, a[i]);
        }
        sp[0][block] = cur_max;
    }
    tt();
    build(LOG, 0, 1 << LOG);
    tt();

    calculate_precalc(n);
    tt();

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
        ans = query(min(l, r), max(l, r));
        total += ans;
        x = ((x * 1103515245LL + 12345) % MOD);
    }
    cout << total << "\n";
    tt();
    return 0;
}
