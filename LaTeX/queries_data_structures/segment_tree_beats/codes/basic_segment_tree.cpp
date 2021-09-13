\begin{code}
void update(int node, int l, int r, int ql, int qr, int newVal) {
    if (qr <= l || r <= ql) { // node is outside of the segment
        return;
    }
    if (ql <= l && r <= qr) { // node is inside the segment
        updateNode(node, newVal);
        setPush(node, newVal);
        return;
    }
    // node intersects the segment
    pushDown(node);
    int mid = (r + l) / 2;
    update(2 * node, l, mid, ql, qr, newVal);
    update(2 * node + 1, mid, r, ql, qr, newVal);
    pullUp(node);
}
\end{code}
