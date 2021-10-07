\begin{code}
vector<int> get_all_modular_inverses(int p) {
    vector<int> inverse_factorials(p);
    inverse_factorials[p - 1] = p - 1; // -1 mod p = p - 1
    for (int k = p - 2; k > 0; k--) {
        inverse_factorials[k] = 1LL * inverse_factorials[k + 1] * (k + 1) % p;
    }

    vector<int> inverses(p);
    int factorial = 1;
    for (int k = 1; k < p; k++) {
        inverses[k] = 1LL * factorial * inverse_factorials[k] % p;
        factorial = 1LL * factorial * k % p;
    }
    return inverses;
}
\end{code}
