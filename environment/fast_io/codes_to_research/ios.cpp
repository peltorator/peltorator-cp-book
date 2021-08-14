#include <bits/stdc++.h>

using namespace std;

int main() {
    freopen("out.txt", "w", stdout);
    ios::sync_with_stdio(0);
    cin.tie(0);
    int T;
    cin >> T;
    vector<int> a(T);
    long long sum = 0;
    for (int i = 0; i < T; i++) {
        cin >> a[i];
        sum += a[i];
    }
    cout << sum << '\n';
    cerr << T << endl;
    cerr << "Time: " << clock() * 1.0 / CLOCKS_PER_SEC << endl;
}
// 14.14
