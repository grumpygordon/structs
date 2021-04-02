template<typename T = int>
struct Dinic {
    struct Edge {
        int v, u;
        T f, c;
    };
    vector<Edge> ed;
    vector<vector<int> > e;
    int n, st, fin;
    T flow;

    vector<int> ptr, ds, que;

    Dinic(int n_, int st_, int fin_) : n(n_), st(st_), fin(fin_), flow(0) {
        e.resize(n);
        ptr.resize(n);
        ds.resize(n);
        que.resize(n);
    }

    void add(int v, int u, T c, T bc = 0, T f = 0) {
        int w = int(ed.size());
        ed.push_back({v, u, f, c});
        ed.push_back({u, v, -f, bc});
        e[v].push_back(w);
        e[u].push_back(w + 1);
    }

    bool bfs(T scale) {
        fill(ds.begin(), ds.end(), -1);
        ds[st] = 0;
        int sz = 1;
        que[0] = st;
        for (int it = 0; it < sz; it++) {
            int v = que[it];
            for (int i : e[v])
                if (ed[i].f + scale <= ed[i].c && ds[ed[i].u] == -1) {
                    ds[ed[i].u] = ds[v] + 1;
                    if (ed[i].u == fin)
                        return 1;
                    que[sz++] = ed[i].u;
                }
        }
        return 0;
    }

    T dfs(int v, T w, T cs) {
        if (v == fin)
            return w;
        int &i = ptr[v];
        while (i >= 0) {
            auto &t = ed[e[v][i]];
            if (ds[t.u] == ds[v] + 1 && t.f + cs <= t.c) {
                T o = dfs(t.u, min(w, t.c - t.f), cs);
                if (o) {
                    t.f += o;
                    ed[e[v][i] ^ 1].f -= o;
                    return o;
                }
            }
            i--;
        }
        return 0;
    }

    T get(int scale = 30) {
        for (T i = ((T(1)) << scale); i > 0; i >>= 1)
            while (bfs(i)) {
                for (int j = 0; j < n; j++)
                    ptr[j] = int(e[j].size()) - 1;
                while (T w = dfs(st, numeric_limits<T>::max(), i))
                    flow += w;
            }
        return flow;
    }
};