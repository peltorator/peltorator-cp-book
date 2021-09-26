\begin{code}
import os, sys

for i in range(1, 100000):
    print('Running test', i)
    os.popen('./gen > input')
    smart_ans = os.popen('./smart < input').readlines()
    stupid_ans = os.popen('./stupid < input').readlines()
    if smart_ans != stupid_ans:
        print('Outputs are not equal')
        print('Input:')
        print(*(open('input').readlines()))
        print('stupid answer:')
        print(*stupid_ans)
        print('smart answer:')
        print(*smart_ans)
        sys.exit()
print('All tests passed')
\end{code}
