
vector<int> e[maxn];

int n;

struct LCA {
    int up[LG][maxn];
    int h[maxn];

    void dfs(int v, int par = -1) {
        for (int i : e[v])
            if (i != par) {
                h[i] = h[v] + 1;
                up[0][i] = v;
                dfs(i, v);
            }
    }

    LCA() {
        up[0][0] = 0;
        dfs(0);
        for (int j = 1; j < LG; j++)
            for (int i = 0; i < n; i++)
                up[j][i] = up[j - 1][up[j - 1][i]];
    }

    int get(int v, int u) {
        if (h[v] < h[u])
            swap(v, u);
        for (int i = LG - 1; i >= 0; i--)
            if (h[v] - h[u] >= (1 << i))
                v = up[i][v];
        if (v == u)
            return v;
        for (int i = LG - 1; i >= 0; i--)
            if (up[i][v] != up[i][u]) {
                v = up[i][v];
                u = up[i][u];
            }
        return up[0][v];
    }

    int par(int v) {
        return up[0][v];
    }
};
