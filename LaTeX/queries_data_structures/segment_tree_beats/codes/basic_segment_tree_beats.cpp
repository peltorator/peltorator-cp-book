\begin{code}
void update(int node, int l, int r, int ql, int qr, int newval) {
    if (break_condition(node, ql, qr, newVal)) {
        return;
    }
    if (tag_condition(node, ql, qr, newval)) {
        update_node(node, newval);
        set_push(node, newval);
        return;
    }
    push_down(node);
    int mid = (r + l) / 2;
    update(2 * node, l, mid, ql, qr, newval);
    update(2 * node + 1, mid, r, ql, qr, newval);
    pull_up(node);
}
\end{code}
