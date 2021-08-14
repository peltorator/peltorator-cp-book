\begin{code}
int findMin(int l, int r) { // [l, r)
    int power = logs[r - l];
    return min(sparse[power][l], sparse[power][r - (1 << power)]);
}
\end{code}
