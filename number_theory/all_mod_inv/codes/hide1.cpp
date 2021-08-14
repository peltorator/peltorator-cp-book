\begin{code}
vector<int> getAllModularInverse(int p) {
    vector<int> inverseFactorials(p);
    inverseFactorials[p - 1] = p - 1; // -1 mod p = p - 1
    for (int k = p - 2; k > 0; k--) {
        inverseFactorials[k] = 1LL * inverseFactorials[k + 1] * (k + 1) % p;
    }

    vector<int> inverses(p);
    int factorial = 1;
    for (int k = 1; k < p; k++) {
        inverses[k] = 1LL * factorial * inverseFactorials[k] % p;
        factorial = 1LL * factorial * k % p;
    }
    return inverses;
}
\end{code}

А также максимально соптимизированный по времени алгоритм:

\begin{code}
int inverses[N]; // N >= p

void get_revs(int p) {
    inverses[p - 1] = p - 1;
    for (int k = p - 2; k > 0; k--) {
        inverses[k] = 1LL * inverses[k + 1] * (k + 1) % p;
        // we store inverseFactorials in the same array as revs
    }

    ll fact = 1;
    for (int k = 1; k < p; k++) {
        revs[k] = fact * revs[k] % p;
        fact = fact * k % p;
    }
}
\end{code}
