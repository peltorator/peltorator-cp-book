const int LGN = 10, N = 500, LGM = 10, M = 500;
 
int logs[max(N, M)];
int sparse[LGN][LGM][N][M];
 
int find(int lx, int rx, int ly, int ry)  // [lx; rx) x [ly; ry)
{
    int x = logs[rx - lx], y = logs[ry - ly];
    return max({sparse[x][y][lx][ly], sparse[x][y][rx - (1 << x)][ly],
                sparse[x][y][lx][ry - (1 << y)], sparse[x][y][rx - (1 << x)][ry - (1 << y)]});
}
 
void buildsparse(vector<vector<int>> &a)
{
    int n = a.size();
    int m = a[0].size();
    for (int i = 2; i < max(n, m); i++) {
        logs[i] = logs[i >> 1] + 1;
    }
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < m; y++) {
            sparse[0][0][x][y] = a[x][y];
        }
    }
    for (int lgx = 0; lgx + 1 < LGN; lgx++) {
        for (int x = 0; x + (1 << lgx) < n; x++) {
            for (int y = 0; y < m; y++) {
               sparse[lgx + 1][0][x][y] = max(sparse[lgx][0][x][y], sparse[lgx][0][x + (1 << lgx)][y]);
            }
        }
    }
    for (int lgx = 0; lgx + 1 < LGN; lgx++)
        for (int lgy = 0; lgy + 1 < LGM; lgy++)
            for (int x = 0; x < n; x++)
                for (int y = 0; y + (1 << lgy) < m; y++)
                    sparse[lgx][lgy + 1][x][y] = max(sparse[lgx][lgy][x][y], sparse[lgx][lgy][x][y + (1 << lgy)]);
}
