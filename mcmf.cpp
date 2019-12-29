template<typename F = int, typename C = int>
struct mcmf {
    struct Edge {
        int v, u;
        F f, c;
        C cost;
    };
    vector<Edge> ed;
    vector<vector<int> > e;
    int n, st, fin;
    F flow;
    C cost;

    vector<C> pot, npot;
    vector<int> par, tak;

    priority_queue<pair<C, int> > que;

    mcmf(int n_, int st_, int fin_) : n(n_), st(st_), fin(fin_), flow(0), cost(0) {
        e.resize(n);
        pot.resize(n);
        npot.resize(n);
        par.resize(n);
        tak.resize(n);
    }

    void add(int v, int u, F c, C cost_, F bc = 0, F f = 0) {
        int w = ed.size();
        ed.push_back({v, u, f, c, cost_});
        ed.push_back({u, v, -f, bc, -cost_});
        e[v].push_back(w);
        e[u].push_back(w + 1);
    }

    bool ford_bellman() {
        const C INF = numeric_limits<C>::max();
        fill(pot.begin(), pot.end(), INF);
        pot[st] = 0;
        bool upd;
        for (int i = 0; i < n; i++) {
            upd = 0;
            for (int j1 = 0; j1 < int(ed.size()); j1++) {
                auto &j = ed[j1];
                if (pot[j.v] != INF && j.f < j.c && pot[j.u] > pot[j.v] + j.cost) {
                    pot[j.u] = pot[j.v] + j.cost;
                    par[j.u] = j1;
                    upd = 1;
                }
            }
            if (!upd)
                break;
        }
        assert(!upd);
        return pot[fin] != INF;
    }

    bool dijka(bool use_que) {
        const C INF = numeric_limits<C>::max();
        if (!use_que) {
            fill(tak.begin(), tak.end(), 0);
            fill(npot.begin(), npot.end(), INF);
            npot[st] = 0;
            for (int it = 0; it < n; it++) {
                int v = -1;
                C vw = INF;
                for (int i = 0; i < n; i++)
                    if (!tak[i] && vw > npot[i])
                        v = i, vw = npot[i];
                if (v == -1)
                    break;
                tak[v] = 1;
                for (int i : e[v])
                if (ed[i].f < ed[i].c) {
                    auto &w = ed[i];
                    assert(pot[w.u] != INF);
                    assert(pot[v] != INF);
                    C uw = w.cost + pot[v] - pot[w.u];
                    assert(uw >= 0);
                    uw += npot[v];
                    if (npot[w.u] > uw) {
                        npot[w.u] = uw;
                        par[w.u] = i;
                    }
                }
            }
            for (int i = 0; i < n; i++)
                pot[i] = (npot[i] == INF ? INF : pot[i] + npot[i]);
        } else {
            fill(tak.begin(), tak.end(), 0);
            fill(npot.begin(), npot.end(), INF);
            npot[st] = 0;
            que.push({0, st});
            while (!que.empty()) {
                int v = que.top().sc;
                que.pop();
                if (tak[v])
                    continue;
                tak[v] = 1;
                for (int i : e[v])
                if (ed[i].f < ed[i].c) {
                    auto &w = ed[i];
                    assert(pot[w.u] != INF);
                    assert(pot[v] != INF);
                    C uw = w.cost + pot[v] - pot[w.u];
                    assert(uw >= 0);
                    uw += npot[v];
                    if (npot[w.u] > uw) {
                        npot[w.u] = uw;
                        que.push({-uw, w.u});
                        par[w.u] = i;
                    }
                }
            }
            for (int i = 0; i < n; i++)
                pot[i] = (npot[i] == INF ? INF : pot[i] + npot[i]);
        }
        return pot[fin] != INF;
    }

    pair<F, C> get(bool use_que = 0) {
        if (!ford_bellman())
            return {0, 0};
        while (1) {
            vector<int> q;
            int v = fin;
            F fl = numeric_limits<F>::max();
            C cs = 0;
            while (v != st) {
                q.push_back(par[v]);
                auto &w = ed[par[v]];
                fl = min(fl, w.c - w.f);
                cs += w.cost;
                v = w.v;
            }
            // cerr << fl << ' ' << cs << '\n';
            flow += fl;
            cost += fl * cs;
            for (int &i : q)
                ed[i].f += fl, ed[i ^ 1].f -= fl;
            if (!dijka(use_que))
                return {flow, cost};
        }
    }
};
