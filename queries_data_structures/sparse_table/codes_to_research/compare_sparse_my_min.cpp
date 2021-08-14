#include <bits/stdc++.h>

using namespace std;

#define mn(x, y) (x < y ? x : y)

const int N = 1e6;
const int LG = 20;

int sp1[LG][N];
int sp2[LG][N];
int sp3[N][LG];
int sp4[N][LG];

int arr[N];

mt19937 rnd(239);

double st;

void strt() {
    st = clock() * 1.0 / CLOCKS_PER_SEC;
}

void fin() {
    cout << "Time " << clock() * 1.0 / CLOCKS_PER_SEC - st << endl;
}

int main() {
    for (int i = 0; i < N; i++) {
        arr[i] = rnd();
    }

    strt();
    for (int i = 0; i < N; i++) {
        sp1[0][i] = arr[i];
    }
    for (int j = 0; j + 1 < LG; j++) {
        for (int i = 0; i + (1 << j) < N; i++) {
            sp1[j + 1][i] = mn(sp1[j][i], sp1[j][i + (1 << j)]);
        }
    }
    fin();

    strt();
    for (int i = 0; i < N; i++) {
        sp2[0][i] = arr[i];
    }
    for (int i = N - 1; i >= 0; i--) {
        for (int j = 0; (1 << j) + i < N && j + 1 < LG; j++) {
            sp2[j + 1][i] = mn(sp2[j][i], sp2[j][i + (1 << j)]);
        }
    }
    fin();


    strt();
    for (int i = 0; i < N; i++) {
        sp3[0][i] = arr[i];
    }
    for (int j = 0; j + 1 < LG; j++) {
        for (int i = 0; i + (1 << j) < N; i++) {
            sp3[i][j + 1] = mn(sp3[i][j], sp3[i + (1 << j)][j]);
        }
    }
    fin();

    strt();
    for (int i = 0; i < N; i++) {
        sp4[0][i] = arr[i];
    }
    for (int i = N - 1; i >= 0; i--) {
        for (int j = 0; (1 << j) + i < N && j + 1 < LG; j++) {
            sp4[i][j + 1] = mn(sp4[i][j], sp4[i + (1 << j)][j]);
        }
    }
    fin();

}
