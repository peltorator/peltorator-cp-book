#include <bits/stdc++.h>

using namespace std;

const int N = 30;

int jump[N];

int mylg(int x) {
    return 31 - __builtin_clz(x);
}

int main() {
    freopen("out.txt", "w", stdout);
    cout << "1\t0\n";
    for (int i = 1; i < N; i++) {
        if (i - 1 - jump[i - 1] == jump[i - 1] - jump[jump[i - 1]]) {
            jump[i] = jump[jump[i - 1]];
        } else {
            jump[i] = i - 1;
        }
        cout << i + 1 << '\t' << mylg(i - jump[i] + 1) << "\n";
    }
    cout << endl;
    for (int i = 1; i < N; i++) {
        cout <<  mylg(i - jump[i] + 1) << ", ";
    }
}
