// can only modify one node
// modify on path is doable (light_sons.set(v, 0) with lazy propagation, same with tree.modify(l, r))
// modify on subtree is ochen slogno

// you can init in O(n) cherez gopu (everything you need exists)
struct HLD {

    struct heavy_item;

    // you can change Light_sons if necessary
    // light_list:
    // now each list is stored in segtree
    // you can change it to O(1) struct
    // Light_sons:
    // you can store all lists together and do shit
    struct Light_sons {

        Light_sons() {}

        // info from light son
        struct info {
            // TODO

            // added for light_list:segtree
            template<typename T>
            void init(const T &t, int l, int r) {
                *this = t;
            }

            void update(const info &first, const info &second, int l, int r) {
                // TODO
            }

            static info merge(const info &first, const info &second, int l, int r) {
                info res;
                res.update(first, second, l, r);  // careful with different lengths
                return res;
            }
        };

        struct light_list {

            struct segtree {
                vector<info> tree;
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
                    tree[i].update(tree[i * 2 + 1], tree[i * 2 + 2], l, r);
                }

                template<typename T>
                void build(const vector<T> &v) {
                    n = v.size();
                    tree.assign(1 << (__lg(max(1, n - 1)) + 2), info());
                    build(v, 0, 0, n - 1);
                }

                info ask(int l, int r, int i, int vl, int vr) {
                    if (l == vl && r == vr) {
                        return tree[i];
                    }
                    int m = (vl + vr) >> 1;
                    if (r <= m) {
                        return ask(l, r, i * 2 + 1, vl, m);
                    } else if (m < l) {
                        return ask(l, r, i * 2 + 2, m + 1, vr);
                    } else {
                        return info::merge(ask(l, m, i * 2 + 1, vl, m), ask(m + 1, r, i * 2 + 2, m + 1, vr), l, r);
                    }
                }

                info ask(int l, int r) {
                    l = max(l, 0);
                    r = min(r, n - 1);
                    if (l > r) return info();
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
                        tree[i].update(tree[i * 2 + 1], tree[i * 2 + 2], st[ptr].first, st[ptr].second);
                        --ptr;
                    }
                }
            };

            segtree t;

            void init(vector<info> const &q) {
                t.build(q);
            }

            info get() {
                return t.ask(0, inf);
            }

            void set(int i, info const &w) {
                t.set(i, w);
            }
        };

        // light son info
        vector<light_list> tlight;

        void init(int n) {
            tlight.assign(n, {});
        }


        void init_list(int v, int n) {
            tlight[v].init(vector<info>(n));
        }

        void init_list(int v, vector<info> const &q) {
            tlight[v].init(q);
        }

        info get(int v) {
            return tlight[v].get();
        }

        // id - index in sons[v]
        void set(int v, int id, info const &w) {
            tlight[v].set(id, w);
        }
    };

    // aggregate myself (light_list[v][0]) and light sons
    // to get item for heavy path
    // NOTE: light_son already has up edge (if you are yourself, then no edge needed), it was added in heavy_to_light
    heavy_item light_to_heavy(int v) {
        auto w = light_sons.get(v);

        heavy_item s{};
        // dont change
        s.edge = p_edge[v];

        // TODO s from w
        return s;
    }

    // convert heavy item to light so I can push it to my father
    // u must be light son (= root of heavy path and not root)
    // NOTE: you must add up edge
    Light_sons::info heavy_to_light(int u) {
        auto s = tree.get(u);
        assert(u != root && hid[u] == u);
        int v = p[u];
        int e = p_edge[u];
        // add e

        Light_sons::info w{};
        // TODO w from s
        return w;
    }

    // heavy_item for heavy path
    struct heavy_item {
        // edge to the left, required at update
        int edge = 0;
        // sum of edges in segment, required at update
        ll sum = 0;

        // TODO example below
        // l = closest to left
        // r = closest to right
        ll l = inf, r = inf, ans = inf;

        void init(const heavy_item &t, int, int) {
            *this = t;
        }

        void update(const heavy_item &first, const heavy_item &second) {
            // dont delete, required
            edge = first.edge;
            sum = first.sum + second.edge + second.sum;

            // use it
            int e = second.edge;

            // TODO example below
            ans = min(first.ans, second.ans);
            setmin(ans, first.r + second.l + e);

            l = min(first.l, first.sum + e + second.l);
            r = min(second.r, second.sum + e + first.r);
        }

        static heavy_item merge(const heavy_item &first, const heavy_item &second) {
            heavy_item res;
            res.update(first, second);  // careful with different lengths
            return res;
        }
    };

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

        vector<segtree> t;

        void init(int n) {
            init(vector<heavy_item>(n));
        }

        void init(vector<heavy_item> const &q) {
            t.assign(q.size(), {});
            vector<heavy_item> g(q.size());
            for (int i = 0; i < q.size(); i++)
                g[tin[i]] = q[i];
            vector<vector<heavy_item>> cur(q.size());
            for (int i = 0; i < q.size(); i++) {
                // hseg[i].fr .. hseg[i].sc
                for (int j = hseg[i].fr; j <= hseg[i].sc; j++)
                    cur[i].emplace_back(g[j]);
            }
            for (int i = 0; i < cur.size(); i++)
                if (!cur[i].empty())
                    t[i].build(cur[i]);
                else
                    t[i].n = 0;
        }

        void init_heavy_path(int v, vector<heavy_item> const &q) {
            // v - id of path (also root)
            assert(hid[v] == v);
            assert(q.size() == hseg[v].sc - hseg[v].fr + 1);
            t[v].build(q);
        }

        void modify(int v, heavy_item const &w) {
            t[hid[v]].set(hpos[v], w);
        }

        template<typename T>
        void modify_path(int v, int u, T const &w) {
            // implement lazy_propagation
            assert(0);
        }

        template<typename T>
        void modify(int l, int r, T const &w) {
            // if you need lazy propagation change segtree to segtree_lazy
            // t.modify(l, r, w);

            // in this version of HLD_segtree you can only change things on heavy path
            // refer to get_path
            assert(0);
        }

        heavy_item get(int v) {
            // TODO
            // usually v is always root of heavy path
            // if segtree is bottom up you can calculate it in O(1) instead of log
            return t[hid[v]].ask(hpos[v], inf);
        }

        heavy_item get_path(int v, int u) {
            assert(hid[v] == hid[u]);
            int id = hid[v];
            v = hpos[v];
            u = hpos[u];
            if (v > u)
                swap(v, u);
            return t[id].ask(v, u);
        }

        heavy_item get_segm(int l, int r) {
            // not doable in this version
            assert(0);
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

    Light_sons light_sons{};
    HLD_segtree tree{hid, hpos, tin, hseg};

    vector<ll> hei;

    // p[i] - parent
    vector<int> p;

    // weight of edge to parent
    vector<int> p_edge;

    // sons, no parent
    vector<vector<pair<int, int>>> e;

    int root;

    // which id am i at my light parent
    vector<int> light_id;

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
            p_edge[i] = hlen;
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
        for (int i = 1; i < e[v].size(); i++)
            light_id[e[v][i].fr] = i;
        {
            int siz = 1;
            if (!e[v].empty())
                siz = e[v].size();
            light_sons.init_list(v, siz);
        }
        for (auto[i, _] : e[v])
            if (i != e[v][0].fr)
                idfs(i, i);
            else
                idfs(i, heavy_id);
    }

    void propagate(int v) {
        while (1) {
            tree.modify(v, light_to_heavy(v));
            v = hid[v];
            if (v == root)
                break;
            // prop to p[v]
            light_sons.set(p[v], light_id[v], heavy_to_light(v));
            v = p[v];
        }
    }

    void change_edge(int v, int u, int w) {
        if (tin[v] > tin[u])
            swap(v, u);
        assert(p[u] == v);
        p_edge[u] = w;
        // p_edge is used in light_to_heavy
        propagate(u);
    }
    
    // TODO col or what you need
    vector<int> col;

    template<typename T>
    void change_node(int v, T const &color) {
        Light_sons::info s{};

        // fill s and update other vectors (col for example)
        // TODO

        // dont change that
        light_sons.set(v, 0, s);
        propagate(v);
    }

    heavy_item get_ans() {
        return tree.get(root);
    }

    bool anc(int v, int u) {
        return tin[v] <= tin[u] && tin[v] + sz[v] > tin[u];
    }

    // TODO: if you need more functions (get, update on path), take from regular hld

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
            hseg.assign(n, {0, -1});
            hpath.assign(n, {});
            p.assign(n, -1);
            p_edge.assign(n, 0);
            h.assign(n, 0);
            sz.assign(n, 0);
            hei.assign(n, 0);
            light_id.assign(n, -1);
            light_sons.init(n);

            // TODO what you need
            col.assign(n, -1);
        }
        calcsz(root);
        idfs(root, root);
        tree.init(n);

        // TODO: O(n) init can be done here
        // TODO: fill other vectors you need (col and others)
    }
};
