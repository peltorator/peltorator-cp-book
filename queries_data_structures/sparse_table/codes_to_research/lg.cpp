#include <bits/stdc++.h>

using namespace std;

int main() {
    for (int i = 1; i <= 10; i++) {
        cerr << ffs(i) << ' ' << __builtin_ctz(i) << endl;
    }
    for (int i = 1; i < 10000000; i++) {
        if (ffs(i) != 31 - __builtin_clz(i)) {
            cout << i << endl;
            return 0;
        }
    }
}
