\begin{code}
#include <iostream>
#include <random>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
    int seed = atoi(argv[1]);
    mt19937 rnd(seed);

    int a = rnd() % 1000;
    int b = rnd() % 1000;

    cout << a << ' ' << b << endl;
    return 0;
}
\end{code}
