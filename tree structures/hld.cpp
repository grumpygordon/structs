// can only modify one node
// modify on path is doable (light_sons.set(v, 0) with lazy propagation, same with tree.modify(l, r))
// modify on subtree is ochen slogno

// you can init in O(n) cherez gopu (everything you need exists)
struct HLD {

    // heavy_item for heavy path
    struct heavy_item {
        // TODO example below
        // l = min -h[v]
        // r = min +h[v]
        int l = inf, r = inf, ans = inf;

        template<typename T>
        void init(const T &t, int, int) {
            // TODO
        }

        void update(const heavy_item &first, const heavy_item &second) {
            // TODO
        }

        static heavy_item merge(const heavy_item &first, const heavy_item &second) {
            heavy_item res;
            res.update(first, second);  // careful with different lengths
            return res;
        }
    };

    void add_edge(int v, heavy_item &w) {
        // add edge v->p[v] on top of w
        // w comes from v
        assert(v != root);

        int len = hei[v] - hei[p[v]];
        // TODO
    }

    struct HLD_segtree {

        vector<int> const &hid, &hpos, &tin;
        vector<pair<int, int>> const &hseg;

        HLD_segtree(vector<int> const &_hid, vector<int> const &_hpos, vector<int> const &_tin,
                    vector<pair<int, int>> const &_hseg) : hid(_hid), hpos(_hpos), tin(_tin), hseg(_hseg) {}

        struct segtree {
            vector<heavy_item> tree;
            int n = 1;

            segtree(int n = 1) : n(n) {
                tree.resize(1 << (__lg(max(1, n - 1)) + 2));
            }

            template<typename T>
            segtree(const vector<T> &v) {
                build(v);
            }

            template<typename T>
            void build(const vector<T> &v, int i, int l, int r) {
                if (l == r) {
                    tree[i].init(v[l], l, r);
                    return;
                }
                int m = (l + r) >> 1;
                build(v, i * 2 + 1, l, m);
                build(v, i * 2 + 2, m + 1, r);
                tree[i].update(tree[i * 2 + 1], tree[i * 2 + 2]);
            }

            template<typename T>
            void build(const vector<T> &v) {
                n = v.size();
                tree.assign(1 << (__lg(max(1, n - 1)) + 2), heavy_item());
                build(v, 0, 0, n - 1);
            }

            heavy_item ask(int l, int r, int i, int vl, int vr) {
                if (l == vl && r == vr) {
                    return tree[i];
                }
                int m = (vl + vr) >> 1;
                if (r <= m) {
                    return ask(l, r, i * 2 + 1, vl, m);
                } else if (m < l) {
                    return ask(l, r, i * 2 + 2, m + 1, vr);
                } else {
                    return heavy_item::merge(ask(l, m, i * 2 + 1, vl, m), ask(m + 1, r, i * 2 + 2, m + 1, vr));
                }
            }

            heavy_item ask(int l, int r) {
                l = max(l, 0);
                r = min(r, n - 1);
                if (l > r) return heavy_item();
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
                tree[i].init(t, l, r);
                while (i != 0) {
                    i = (i - 1) / 2;
                    tree[i].update(tree[i * 2 + 1], tree[i * 2 + 2]);
                    --ptr;
                }
            }
        };

        segtree t;

        void init(int n) {
            t = segtree{n};
        }

        template<typename T>
        void init(vector<T> const &q) {
            vector<T> g(q.size());
            for (int i = 0; i < q.size(); i++)
                g[tin[i]] = q[i];
            t.build(g);
        }

        template<typename T>
        void init_heavy_path(int v, vector<T> const &q) {
            // v - id of path (also root)
            assert(hid[v] == v);
            assert(q.size() == hseg[v].sc - hseg[v].fr + 1);
            for (int i = 0; i < q.size(); i++)
                t.set(hseg[v].fr + i, q[i]);
        }

        template<typename T>
        void modify(int v, T const &w) {
            t.set(tin[v], w);
        }

        template<typename T>
        void modify_path(int v, int u, T const &w) {
            // see modify below
            assert(0);
        }

        template<typename T>
        void modify(int l, int r, T const &w) {
            // if you need lazy propagation change segtree to segtree_lazy
            // t.modify(l, r, w);
            assert(0);
        }

        heavy_item get(int v) {
            return t.ask(tin[v], hseg[hid[v]].sc);
        }

        heavy_item get_path(int v, int u) {
            assert(hid[v] == hid[u]);
            v = tin[v];
            u = tin[u];
            if (v > u)
                swap(v, u);
            return t.ask(v, u);
        }

        heavy_item get_segm(int l, int r) {
            return t.ask(l, r);
        }
    };

    int n;

    // hid[v] - which heavy path v is on
    vector<int> hid;
    // hpos[v] - index of v at his heavy path hid[v]
    vector<int> hpos;
    // tin[v] - id of v at segment tree
    vector<int> tin;
    // hseg[i] - segment at segment tree of heavy path i
    vector<pair<int, int>> hseg;
    // hpath[i] - list of nodes in heavy path i
    vector<vector<int>> hpath;

    // layout in segment tree
    // tin is inverse of ord
    vector<int> ord;

    // size
    vector<int> sz;

    // height
    vector<int> h;

    HLD_segtree tree{hid, hpos, tin, hseg};

    vector<ll> hei;

    // p[i] - parent
    vector<int> p;

    // sons, no parent
    vector<vector<pair<int, int>>> e;

    int root;

    void calcsz(int v, int par = -1) {
        p[v] = par;
        {
            for (int it = 0; it < e[v].size(); it++)
                if (e[v][it].fr == par) {
                    e[v].erase(e[v].begin() + it);
                    break;
                }
        }
        sz[v] = 1;
        for (auto[i, hlen] : e[v]) {
            h[i] = h[v] + 1;
            hei[i] = hei[v] + hlen;
            calcsz(i, v);
            sz[v] += sz[i];
        }
        for (int i = 1; i < e[v].size(); i++)
            if (sz[e[v][0].fr] < sz[e[v][i].fr])
                swap(e[v][0], e[v][i]);
    }


    void idfs(int v, int heavy_id) {
        // heavy_id = hid[v] = index of top node of path
        tin[v] = ord.size();
        if (v == heavy_id)
            hseg[heavy_id].fr = ord.size();
        hseg[heavy_id].sc = ord.size();
        ord.emplace_back(v);
        hid[v] = heavy_id;
        hpos[v] = hpath[heavy_id].size();
        hpath[heavy_id].emplace_back(v);
        for (auto[i, _] : e[v])
            if (i != e[v][0].fr)
                idfs(i, i);
            else
                idfs(i, heavy_id);
    }

    heavy_item get_vertical(int v, int u) {
        assert(anc(v, u));
        heavy_item w{};
        while (1) {
            if (hid[v] == hid[u]) {
                w = heavy_item::merge(tree.get_path(v, u), w);
                return w;
            }
            w = heavy_item::merge(tree.get_path(hid[u], u), w);
            u = hid[u];
            add_edge(u, w);
            u = p[u];
        }
    }

    auto merge_verticals(int v, heavy_item const &a, heavy_item const &b) {
        // two vertical paths a and b end on the same node (node is not shared, only in a)
        // b might be empty
        // TODO example below
        int ans = min(a.ans, b.ans);
        if (a.r != inf && b.r != inf)
            setmin(ans, a.r + b.r - 2 * (int)hei[v]);
        return ans;
        // returns answer to your task
    }

    auto get_path(int v, int u) {
        if (tin[v] > tin[u])
            swap(v, u);
        heavy_item w{};
        if (anc(v, u)) {
            // v is lca of u
            swap(u, v);
        } else {
            while (tin[v] < tin[u]) {
                assert(tin[hid[u]] > tin[v]);
                w = heavy_item::merge(tree.get_path(hid[u], u), w);
                u = hid[u];
                add_edge(u, w);
                u = p[u];
            }
        }
        // u is lca of v
        return merge_verticals(u, get_vertical(u, v), w);
    }

    auto get_subtree(int v) {
        return tree.get_segm(tin[v], tin[v] + sz[v] - 1);
    }


    // TODO col or what you need
    vector<int> col;

    template<typename T>
    void change_node(int v, T const &color) {
        // TODO example below
        tree.modify(v, make_pair(color, hei[v]));
    }

    bool anc(int v, int u) {
        return tin[v] <= tin[u] && tin[v] + sz[v] > tin[u];
    }

    template<typename T>
    void change_path(int v, int u, T const &color) {
        if (tin[v] > tin[u])
            swap(v, u);
        if (anc(v, u)) {
            // v is lca of u
            swap(v, u);
        } else {
            while (tin[v] < tin[u]) {
                assert(tin[hid[u]] > tin[v]);
                tree.modify_path(hid[u], u, color);
                u = hid[u];
                u = p[u];
            }
        }
        assert(anc(u, v));
        // u is lca of v
        swap(v, u);
        // v is lca of u
        while (1) {
            if (hid[v] == hid[u]) {
                tree.modify_path(v, u, color);
                break;
            }
            tree.modify_path(hid[u], u, color);
            u = hid[u];
            u = p[u];
        }
    }

    template<typename T>
    void change_subtree(int v, T const &color) {
        tree.modify(tin[v], tin[v] + sz[v] - 1, color);
    }

    // 0-based
    void init(vector<tuple<int, int, int>> const &edges, int root_) {
        n = edges.size() + 1;
        root = root_;
        e.assign(n, {});
        for (auto[v, u, w] : edges) {
            assert(0 <= v && v < n);
            assert(0 <= u && u < n);
            e[v].emplace_back(u, w);
            e[u].emplace_back(v, w);
        }
        {
            ord.clear();
            hid.assign(n, 0);
            hpos.assign(n, 0);
            tin.assign(n, 0);
            hseg.assign(n, {-1, -1});
            hpath.assign(n, {});
            p.assign(n, -1);
            h.assign(n, 0);
            sz.assign(n, 0);
            hei.assign(n, 0);

            // TODO what you need
            col.assign(n, -1);
        }
        calcsz(root);
        idfs(root, root);
        tree.init(n);

        // TODO: O(n) init can be done here
    }
};
