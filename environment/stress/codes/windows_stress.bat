\begin{code}
g++ -std=c++17 -O2 smart.cpp -o smart.exe
g++ -std=c++17 -O2 stupid.cpp -o stupid.exe
g++ -std=c++17 -O2 gen.cpp -o gen.exe

:beg
gen.exe > input || exit
smart.exe < input > smart_output
stupid.exe < input > stupid_output
fc smart_output stupid_output
if errorlevel 1 goto bug
goto beg
:bug
echo found!
\end{code}
