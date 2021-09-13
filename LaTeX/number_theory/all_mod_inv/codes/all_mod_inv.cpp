\begin{code}
vector<int> getAllModularInverses(int p) {
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
