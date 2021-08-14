\begin{code}
void update(int node, int l, int r, int ql, int qr, int newVal) {
    if (breakCondition(node, ql, qr, newVal)) {
        return;
    }
    if (tagCondition(node, ql, qr, newVal)) {
        updateNode(node, newVal);
        setPush(node, newVal);
        return;
    }
    pushDown(node);
    int mid = (r + l) / 2;
    update(2 * node, l, mid, ql, qr, newVal);
    update(2 * node + 1, mid, r, ql, qr, newVal);
    pullUp(node);
}
\end{code}
