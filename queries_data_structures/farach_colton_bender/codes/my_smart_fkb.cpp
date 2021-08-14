#pragma GCC target("sse4")
#include <bits/stdc++.h>

using namespace std;

const int K = 32;
const int N = 20000064;
const int BLOCKS = 625101;
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

int precalc[N];

int ask_small(int l, int r) {
    if (l >= r) {
        return numeric_limits<int>::min();
    }
    int len = r - l;
    int bt = 32 - __builtin_clz(precalc[r] & ((1LL << len) - 1));
    return a[r - bt];
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

int stk[N];

void calculate_precalc(int n) {
    int m = 0;
    int ms = 0;
    for (int i = 0; i < n; i++) {
        precalc[i] = ms;
        ms <<= 1;
        ms |= 1;
        while (m > 0 && a[stk[m - 1]] < a[i]) {
            if (i - stk[m - 1] < 32) {
                ms ^= (1 << (i - stk[m - 1]));
            }
            m--;
        }
        stk[m] = i;
        m++;
    }
    precalc[n] = ms;
}

const long long MOD = 1LL << 31;

int main() {
    int n, b, c, d;
    cin >> n >> a[0] >> b >> c >> d;
    keks = n;
    for (int i = 1; i < n; i++) {
        b = (int) ((b * 1LL * c + d) % MOD);
        a[i] = a[i - 1] + (1 - 2 * ((b % 239179) & 1));
    }

    for (int block = 0; block < BLOCKS; block++) {
        int l = block * K;
        int r = l + K;
        int cur_max = a[l];
        for (int i = l + 1; i < r; i++) {
            maxi(cur_max, a[i]);
        }
        sp[0][block] = cur_max;
    }

    build(LOG, 0, 1 << LOG); 

    calculate_precalc(n);

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

    return 0;
}
