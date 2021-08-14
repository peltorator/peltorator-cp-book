\begin{code}
logs[1] = 0;
for (int i = 2; i <= n; i++) {
    logs[i] = logs[i >> 1] + 1;
}
\end{code}
