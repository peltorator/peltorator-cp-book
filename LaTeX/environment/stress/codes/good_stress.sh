\begin{code}
g++ -std=c++17 -O2 smart.cpp -o smart
g++ -std=c++17 -O2 stupid.cpp -o stupid
g++ -std=c++17 -O2 gen.cpp -o gen

for t in $(seq 1 100000); do
    echo "Running test $t"
    ./gen $t > input || { echo "gen failed"; exit; }
    ./smart < input > smart_output || { echo "smart failed"; exit; }
    ./stupid < input > stupid_output || { echo "stupid failed"; exit; }
    diff smart_output stupid_output || { echo "outputs aren't equal"; exit; }
done
\end{code}
