// call init, add_edge, precalc, calc
// no back edges expected, changable (add_edge and prec)
// any array can be passed for each heavy path (hih, cih)
struct HLD {

    struct item {
        int l = inf, r = -1, w = inf;

        template<typename T>
        void init(const T &t, int le) {
            w = inf;
            if (t) {
                l = r = le;
            } else {
                l = inf;
                r = -1;
            }
        }

        void update(const item &first, const item &second, int l_, int r_) {
            w = min({first.w, second.w});
            if (first.r != -1 && second.r != -1) {
                cerr << "upd " << second.l << ' ' << first.r << ' ' << second.l - first.r << '\n';
                setmin(w, second.l - first.r);
            }
            l = min(first.l, second.l);
            r = max(first.r, second.r);
        }

        static item merge(const item &first, const item &second, int l, int r) {
            item res;
            res.update(first, second, l, r);  // careful with different lengths
            return res;
        }
    };

    struct segtree {
        vector<item> tree;
        vector<int> dep;
        int n = 1;

        segtree(int n = 1) : n(n) {
            tree.resize(1 << (__lg(max(1, n - 1)) + 2));
        }

        void init(int n_, vector<int> &cols, vector<int> &di) {
            n = n_;
            tree.resize(1 << (__lg(max(1, n - 1)) + 2));
            dep = di;
            build(cols);
        }

        template<typename T>
        void build(const vector<T> &v, int i, int l, int r) {
            if (l == r) {
                tree[i].init(v[l], dep[l]);
                return;
            }
            int m = (l + r) >> 1;
            build(v, i * 2 + 1, l, m);
            build(v, i * 2 + 2, m + 1, r);
            tree[i].update(tree[i * 2 + 1], tree[i * 2 + 2], l, r);
        }

        template<typename T>
        void build(const vector<T> &v) {
            n = v.size();
            tree.resize(1 << (__lg(max(1, n - 1)) + 2));
            build(v, 0, 0, n - 1);
        }

        item ask(int l, int r, int i, int vl, int vr) {
            if (l == vl && r == vr) {
                return tree[i];
            }
            int m = (vl + vr) >> 1;
            if (r <= m) {
                return ask(l, r, i * 2 + 1, vl, m);
            } else if (m < l) {
                return ask(l, r, i * 2 + 2, m + 1, vr);
            } else {
                return item::merge(ask(l, m, i * 2 + 1, vl, m), ask(m + 1, r, i * 2 + 2, m + 1, vr), l, r);
            }
        }

        item ask(int l, int r) {
            l = max(l, 0);
            r = min(r, n - 1);
            if (l > r) return item();
            return ask(l, r, 0, 0, n - 1);
        }

        template<typename T>
        void set(int ind, const T &t) {
            static array<pair<int, int>, 30> st;
            int l = 0, r = n - 1, i = 0;
            int ptr = -1;
            while (l != r) {
                st[++ptr] = {l, r};
                int m = (l + r) >> 1;
                if (ind <= m) {
                    i = i * 2 + 1;
                    r = m;
                } else {
                    i = i * 2 + 2;
                    l = m + 1;
                }
            }
            tree[i].init(t, dep[l]);
            while (i != 0) {
                i = (i - 1) / 2;
                tree[i].update(tree[i * 2 + 1], tree[i * 2 + 2], st[ptr].first, st[ptr].second);
                --ptr;
            }
        }
    };

    int n;

    vector<vector<int>> e;

    vector<int> p, sz, tin;

    int tnow = 0;

    // no back edge to parent
    void prec(int v, int par) {
        p[v] = par;
        sz[v] = 1;
        tin[v] = tnow++;
        if (e[v].empty())
            return;
        for (int i : e[v])
            if (i != par) {
                prec(i, v);
                sz[v] += sz[i];
            } else {
                // here
                assert(0);
            }
        int id = 0;
        for (int i = 1; i < e[v].size(); i++)
            if (sz[e[v][id]] < sz[e[v][i]])
                id = i;
        swap(e[v][0], e[v][id]);
    }

    vector<segtree> tree;

    vector<int> hid, hpar, hsz, h;

    void upd(int v, int w) {
        tree[hpar[v]].set(hid[v], w);
    }

    void dfs(int v) {
        hid[v] = hsz[hpar[v]]++;
        if (e[v].empty()) {
            vector<int> he, ce;
            int u = v;
            while (1) {
                // you can pass something for init here
                he.push_back(hih[u]);
                ce.push_back(cih[u]);
                if (u == hpar[v])
                    break;
                u = p[u];
            }
            reverse(all(he));
            reverse(all(ce));
            tree[hpar[v]].init(hsz[hpar[v]], ce, he);
            return;
        }
        hpar[e[v][0]] = hpar[v];
        dfs(e[v][0]);
        for (int i = 1; i < e[v].size(); i++) {
            hpar[e[v][i]] = e[v][i];
            dfs(e[v][i]);
        }
    }

    item get(int v, int u) {
        if (tin[v] < tin[u])
            swap(v, u);
        // u ----> v
        item cur;
        int du = -1;
        while (1) {
            int r = hid[v];
            int l = (hpar[v] == hpar[u] ? hid[u] : 0);
            if (cur.l != inf) {
                int dif = tree[hpar[v]].dep[r] - tree[hpar[v]].dep[l] + du;
                cur.l += dif;
                cur.r += dif;
            }
            cerr << "ask " << l << ' ' << r << '\n';
            auto g = tree[hpar[v]].ask(l, r);
            if (g.l != inf) {
                int dif = tree[hpar[v]].dep[l];
                g.l -= dif;
                g.r -= dif;
            }
            cerr << "merge " << g.l << ' ' << g.r << ' ' << cur.l << ' ' << cur.r << '\n';
            cur = item::merge(g, cur, -1, -1);
            if (hpar[v] == hpar[u])
                break;
            v = hpar[v];
            // du = length of edge p[v] -> v
            du = hih[v] - hih[p[v]];
            v = p[v];
        }
        cerr << "done " << cur.l << ' ' << cur.r << ' ' << cur.w << '\n';
        return cur;
    }

    void init(int n_) {
        n = n_;
        e.resize(n);
        p.resize(n);
        sz.resize(n);
        tin.resize(n);
        tree.resize(n);
        hid.resize(n);
        hpar.resize(n);
        hsz.resize(n);
    }

    void add_edge(int v, int u) {
        // v -> u
        cerr << "add " << v << ' ' << u << '\n';
        e[v].push_back(u);
    }

    void precalc() {
        prec(0, 0);
        dfs(0);
    }

    int calc(int v, int u, int lca) {
        if (tin[v] < tin[u])
            swap(v, u);
        // u --> v
        if (lca == u)
            return get(v, u).w;
        item x = get(v, lca), y = get(u, lca);
        int res = min(x.w, y.w);
        if (x.l != 0 && x.l != inf && y.l != inf)
            setmin(res, x.l + y.l);
        return res;
    }
};
