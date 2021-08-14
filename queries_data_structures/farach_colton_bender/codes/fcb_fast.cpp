#include <bits/stdc++.h>

using namespace std;

namespace FCBRMQ {

    namespace FCBLCA {

        namespace RMQPM1 {
            const int N = 20000004;
            const int K = 12; // log2(N) / 2
            const int BLOCKS = 1666667; // N / K
            const int LG = 21; // log2(BLOCKS)
            const pair<int, int> INF = {2e9 + 7, 0};

            pair<int, int> arr[N];
            pair<int, int> sparse[LG][BLOCKS];
            int precalc[1 << (K - 1)][K][K + 1];
            int blockType[BLOCKS];

            void buildSparse(int n) {
                for (int block = 0; block < BLOCKS; block++) {
                    int l = block * K;
                    int r = min(l + K, n);
                    pair<int, int> blockMin = arr[l];
                    for (int i = l + 1; i < r; i++) {
                        blockMin = min(blockMin, arr[i]);
                        if (arr[i].first == arr[i - 1].first + 1) {
                            blockType[block] |= (1 << (i - l - 1));
                        }
                    }
                    sparse[0][block] = blockMin;
                }

                for (int bit = 1; bit < LG; bit++) {
                    for (int i = 0; i + (1 << bit) <= BLOCKS; i++) {
                        sparse[bit][i] = min(sparse[bit - 1][i], sparse[bit - 1][i + (1 << (bit - 1))]);
                    }
                }
            }

            void buildPrecalc() {
                for (int mask = 0; mask < (1 << (K - 1)); mask++) {
                    for (int l = 0; l < K; l++) {
                        precalc[mask][l][l] = -1;
                        int curmin = numeric_limits<int>::max();
                        int curval = 0;
                        for (int r = l; r < K; r++) {
                            if (curval < curmin) {
                                precalc[mask][l][r + 1] = r;
                                curmin = curval;
                            } else {
                                precalc[mask][l][r + 1] = precalc[mask][l][r];
                            }
                            if ((mask >> r) & 1) {
                                curval++;
                            } else {
                                curval--;
                            }
                        }
                    }
                }
            }

            void init(int n) {
                buildSparse(n);
                buildPrecalc();
            }
          
            pair<int, int> findShort(int left, int right) { // left and right are in one block
                if (left == right) {
                    return INF;
                }
                int block = left / K;
                pair<int, int> ans = arr[block * K + precalc[blockType[block]][left - block * K][right - block * K]];
                return ans;
            }

            pair<int, int> findLong(int lblock, int rblock) {
                if (lblock == rblock) {
                    return INF;
                }
                int bit = 31 - __builtin_clz(rblock - lblock);
                pair<int, int> ans = min(sparse[bit][lblock], sparse[bit][rblock - (1 << bit)]);
                return ans;
            }

            pair<int, int> findMin(int left, int right) { // [left; right)
                int lblock = left / K;
                int rblock = right / K;
                if (lblock == rblock) {
                    return findShort(left, right);
                } else {
                    return min(findLong(lblock + 1, rblock), min(findShort(left, (lblock + 1) * K), findShort(rblock * K, right)));
                }
            }
        }

        const int N = 10000000;

        int left[N];
        int right[N];
        int timein[N];
        pair<int, int> recStack[N];

        int dfs(int root) {
            int stackSize = 1;
            recStack[0] = {0, root};
            timein[0] = 0;
            int eulerSize = 0;
            int lastVertex = root;
            while (stackSize > 0) {
                int curVertex = recStack[stackSize - 1].second;
                int depth = recStack[stackSize - 1].first;
                RMQPM1::arr[eulerSize++] = recStack[stackSize - 1];
                if (right[curVertex] != lastVertex) {
                     if (left[curVertex] != lastVertex && left[curVertex] != -1) {
                        timein[left[curVertex]] = eulerSize;
                        recStack[stackSize++] = {depth + 1, left[curVertex]};
                    } else if (right[curVertex] != -1) {
                        timein[right[curVertex]] = eulerSize;
                        recStack[stackSize++] = {depth + 1, right[curVertex]};
                    } else {
                        stackSize--;
                    }
                } else {
                    stackSize--;
                }
                lastVertex = curVertex;
            }
            return eulerSize;
        }

        void init(int root) {
            int eulerSize = dfs(root);
            RMQPM1::init(eulerSize);
        }

        int findLCA(int u, int v) {
            if (timein[u] > timein[v]) {
                swap(u, v);
            }
            return RMQPM1::findMin(timein[u], timein[v] + 1).second;
        }
    }

    const int N = 10000000;
    int arr[N];
    int rightBranch[N];

    int buildTreap(int n) {
        FCBLCA::left[0] = FCBLCA::right[0] = -1;
        int branchSize = 0;
        rightBranch[branchSize++] = 0;
        for (int i = 1; i < n; i++) {
            FCBLCA::left[i] = FCBLCA::right[i] = -1;
            int lastVertex = -1;
            while (branchSize > 0 && arr[rightBranch[branchSize - 1]] >= arr[i]) {
                lastVertex = rightBranch[--branchSize];
            }
            FCBLCA::left[i] = lastVertex;
            if (branchSize > 0) {
                FCBLCA::right[rightBranch[branchSize - 1]] = i;
            }
            rightBranch[branchSize++] = i;
        }
        return rightBranch[0];
    }

    void init(int n) {
        int root = buildTreap(n);
        FCBLCA::init(root);
    }

    int findMin(int l, int r) {
        return arr[FCBLCA::findLCA(l, r - 1)];
    }
};

// ~~ 18 * n memory
// can me optimized to ~~ 12 * n if we reuse arrays

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> FCBRMQ::arr[i];
    }
    FCBRMQ::init(n);
    int q;
    cin >> q;
    for (int i = 0; i < q; i++) {
        int left, right;
        cin >> left >> right;
        left--; // 0-indexing and half-intervals
        cout << FCBRMQ::findMin(left, right) << '\n';
    }
    return 0;
}
