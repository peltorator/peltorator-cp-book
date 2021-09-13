\begin{code}
const int N = (1 << 20); // N >= n
long long tree[N];
int n;

void build(const vector<int>& arr) {
    n = arr.size();
    for (int i = 0; i < n; i++) {
        tree[n + i] = arr[i];
    }
    for (int i = n - 1; i > 0; i--) {
        tree[i] = 0; // nothing to add on a segment
    }
}

long long findValue(int pos) {
    pos += n;
    long long ans = 0;
    while (pos > 0) {
        ans += tree[pos];
        pos >>= 1;
    }
    return ans;
}

void segmentUpdate(int l, int r, int addVal) { // [l, r)
    l += n;
    r += n;
    while (l < r) {
        if (l & 1) {
            tree[l++] += addVal;
        }
        if (r & 1) {
            tree[--r] += addVal;
        }
        l >>= 1;
        r >>= 1;
    }
}
\end{code}
