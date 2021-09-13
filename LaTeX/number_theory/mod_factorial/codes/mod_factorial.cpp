\begin{code}
int modFactorial(long long n, int p) {
    int factorial = 1;
    while (n > 0) {
        long long k = n / p;
        int b = n % p;
        if (k % 2 == 1) {
            factorial = 1LL * factorial * (p - 1) % p;
        }
        for (int i = 1; i <= b; i++) {
            factorial = 1LL * factorial * i % p;
        }
        n = k;
    }
    return factorial;
}
\end{code}
