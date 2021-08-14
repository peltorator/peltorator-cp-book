#include <bits/stdc++.h>

using namespace std;

mt19937 rnd(239);

unsigned int binaryGCD(unsigned int a, unsigned int b) {
    unsigned int mult = 1;
    while (a && b) {
        if (a & 1) {
            if (!(b & 1)) {
                b >>= 1;
            } else {
                if (a < b) {
                    b = (b - a) >> 1;
                } else {
                    a = (a - b) >> 1;
                }
            }
        } else {
            if (!(b & 1)) {
                mult <<= 1;
                b >>= 1;
            }
            a >>= 1;
        }
    }
    return (a ? a : b) * mult;
}

double getClock() {
    return clock() * 1.0 / CLOCKS_PER_SEC;
}

int main() {
    cout << "Starting time: " << getClock() << endl;
    unsigned int ans = 0;
    for (int i = 1; i <= 100000000; i++) {
        unsigned int x = rnd();
        unsigned int y = rnd();
        ans += binaryGCD(x, y);
    }
    cout << ans << endl;
    cout << "Finish time: " << getClock() << endl;
}

int main3() {
    cout << "Starting time: " << getClock() << endl;
    unsigned int ans = 0;
    for (int i = 1; i <= 10000; i++) {
        for (int j = 1; j <= 10000; j++) {
            ans += binaryGCD(i, j);
        }
    }
    cout << ans << endl;
    cout << "Finish time: " << getClock() << endl;
}

int main2() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n;
    cin >> n;
    while (n--) {
        unsigned int a, b;
        cin >> a >> b;
        cout << binaryGCD(a, b) << '\n';
    }
}
