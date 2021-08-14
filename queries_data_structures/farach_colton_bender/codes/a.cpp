#include <bits/stdc++.h>

using namespace std;

int f1(unsigned i) {
    i |= (i >>  1);
    i |= (i >>  2);
    i |= (i >>  4);
    i |= (i >>  8);
    i |= (i >> 16);
    return i - (i >> 1);
}
 
int f2(unsigned i) {
    union { double a; unsigned b[2]; };
    a = i;
    return (b[1] >> 20) - 1022;
}
 
int f3(unsigned i) {
    return 32 - __builtin_clz(i);
}

int main() {
    for (int i = 1; i <= 10; i++) {
        cout << i << ' ' << f1(i) << ' ' << f2(i) << ' ' << f3(i) << endl;
    }
}
