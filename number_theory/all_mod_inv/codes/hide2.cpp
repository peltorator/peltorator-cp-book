\begin{code}
vector<ll> get_revs(ll p) {
    vector<ll> revs(p);
    revs[1] = 1;
    for (int i = 2; i < p; i++) {
        revs[i] = -(p / i) * revs[p % i] % p + p;
        // +p because this number is negative
    }
    return revs;
}
\end{code}

А также максимально соптимизированный по времени алгоритм:

\begin{code}
int revs[N]; // N >= p

void get_revs(int p) {
    revs[1] = 1;
    for (int i = 2; i < p; i++) {
        revs[i] = -1LL * (p / i) * revs[p % i] % p + p;
    }
}
\end{code}
