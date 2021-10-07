\begin{code}
void update(int node, int l, int r, int ql, int qr, int newval) {
    if (qr <= l || r <= ql) { // node is outside of the segment
        return;
    }
    if (ql <= l && r <= qr) { // node is inside the segment
        update_node(node, newval);
        set_push(node, newval);
        return;
    }
    // node intersects the segment
    push_down(node);
    int mid = (r + l) / 2;
    update(2 * node, l, mid, ql, qr, newval);
    update(2 * node + 1, mid, r, ql, qr, newval);
    pull_up(node);
}
\end{code}
