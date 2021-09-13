\begin{code}
vector<int> getAllModularInverses(int p) {
    vector<int> inverses(p);
    inverses[1] = 1;
    for (int k = 2; k < p; k++) {
        inverses[k] = -1LL * (p / k) * inverses[p % k] % p + p;
        // +p because this number is negative
    }
    return inverses;
}
\end{code}

