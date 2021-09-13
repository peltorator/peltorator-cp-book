\begin{code}
mt19937 rnd(4321);
uniform_int_distribution<int> distrib(1, 10);
cout << distrib(rnd) << endl;
\end{code}
