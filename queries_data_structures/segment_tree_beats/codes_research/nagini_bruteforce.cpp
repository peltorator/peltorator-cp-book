#include <bits/stdc++.h>

using namespace std;

const int MAXN = 100000;
const int INF = 1e9 + 7;
int heightUp[MAXN], heightDown[MAXN];

int main() {
    for (int i = 0; i < MAXN; i++) {
        heightUp[i] = heightDown[i] = INF;
    }
    int q;
    cin >> q;
    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        l--;
        r--;
        if (type == 1) {
            int k;
            cin >> k;
            if (k > 0) {
                for (int i = l; i < r; i++) {
                    heightUp[i] = min(heightUp[i], k);
                }
            } else {
                k = abs(k);
                for (int i = l; i < r; i++) {
                    heightDown[i] = min(heightDown[i], k);
                }
            }
        } else {
            long long ans = 0;
            for (int i = l; i < r; i++) {
                if (heightUp[i] < INF && heightDown[i] < INF) {
                    ans += heightUp[i] + heightDown[i];
                }
            }
            cout << ans << '\n';
        }
    }
    return 0;
}
