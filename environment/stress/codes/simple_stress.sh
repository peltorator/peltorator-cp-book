\begin{code}
g++ -std=c++17 -O2 smart.cpp -o smart
g++ -std=c++17 -O2 stupid.cpp -o stupid
g++ -std=c++17 -O2 gen.cpp -o gen

for t in $(seq 1 100000); do
    echo "Running test $t"
    ./gen $t > input
    ./smart < input > smart_output
    ./stupid < input > stupid_output
    diff smart_output stupid_output || exit
done
\end{code}
