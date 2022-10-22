// source: https://tildeweb.au.dk/au121/papers/mfcs07matching.pdf
// online maximum matching in convex bipartite graph
// operations add/delete segment and a[i]++/a[i]--
// update in log^2(n)
// matching size in O(1)

namespace Brodal_Rakhmatullin {
    // (max, (leftmost_max_pos, rightmost_max_pos))
    namespace segment_tree {
        struct item {
            int v = -inf, u = 0;
            pair<int, int> seg = {-1, -2};

            template<typename T>
            void init(const T &t, int l, int r) {
                v = t;
                seg = {l, l};
            }

            void update(const item &first, const item &second, int l, int r) {
                v = max(first.v, second.v);
                if (first.v > second.v) {
                    seg = first.seg;
                } else if (first.v < second.v) {
                    seg = second.seg;
                } else {
                    seg = {first.seg.fr, second.seg.sc};
                }
            }

            static item merge(const item &first, const item &second, int l, int r) {
                item res;
                res.update(first, second, l, r);  // careful with different lengths
                return res;
            }

            template<typename Modifier>
            void modify(const Modifier &m, int l, int r) {
                v += m;
                u += m;
            }

            void push(item &first, item &second, int l, int r) {
                if (u != 0) {
                    int m = (l + r) / 2;
                    first.modify(u, l, m);
                    second.modify(u, m + 1, r);
                    u = 0;
                }
            }
        };

        struct segtree {
            vector<item> tree;
            int n = 1;

            segtree(int n = 1) : n(n) {
                tree.resize(1 << (__lg(max(1, n - 1)) + 2));
            }

            template<typename T>
            segtree(const vector<T>& v) {
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
                tree.assign(1 << (__lg(max(1, n - 1)) + 2), item());
                build(v, 0, 0, n - 1);
            }

            item ask(int l, int r, int i, int vl, int vr) {
                if (vl != vr) {
                    tree[i].push(tree[i * 2 + 1], tree[i * 2 + 2], vl, vr);
                }
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
                l = max(l, 0); r = min(r, n - 1);
                if (l > r) return item();
                return ask(l, r, 0, 0, n - 1);
            }

            template<typename T>
            void set(int ind, const T &t) {
                static array<pair<int, int>, 30> st;
                int l = 0, r = n - 1, i = 0;
                int ptr = -1;
                while (l != r) {
                    if (l != r) {
                        tree[i].push(tree[i * 2 + 1], tree[i * 2 + 2], l, r);
                    }
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

            template<typename Modifier>
            void modify(int l, int r, const Modifier &modifier, int i, int vl, int vr) {
                if (vl != vr) {
                    tree[i].push(tree[i * 2 + 1], tree[i * 2 + 2], vl, vr);
                }
                if (l == vl && r == vr) {
                    tree[i].modify(modifier, vl, vr);
                    return;
                }
                int m = (vl + vr) >> 1;
                if (r <= m) {
                    modify(l, r, modifier, i * 2 + 1, vl, m);
                } else if (m < l) {
                    modify(l, r, modifier, i * 2 + 2, m + 1, vr);
                } else {
                    modify(l, m, modifier, i * 2 + 1, vl, m);
                    modify(m + 1, r, modifier, i * 2 + 2, m + 1, vr);
                }
                tree[i].update(tree[i * 2 + 1], tree[i * 2 + 2], vl, vr);
            }

            template<typename Modifier>
            void modify(int l, int r, const Modifier &modifier) {
                l = max(l, 0); r = min(r, n - 1);
                if (l > r) return;
                modify(l, r, modifier, 0, 0, n - 1);
            }
        };
    }

    vector<int> arr;

    vector<pair<int, int>> segments;

    struct Prefix {
        int L;
        int n;
        // ((r_bound, (real_r, i))
        set<pair<int, pair<int, int>>> alive, dead;
        segment_tree::segtree t;
        void init(int l, int r) {
            L = l;
            n = r - l + 1;
            assert(n > 0);
            t.build(vector<int>(n, 0));
        }

        pair<int, pair<int, int>> get_segment(int i) {
            pair<int, int> w = {segments[i].sc - L, i};
            return {min(w.fr, n - 1), w};
        }

        // alive -> dead
        int add_val(int i) {
            i -= L;
            assert(i >= 0 && i < n);
            t.modify(i, n - 1, 1);
            auto w = t.ask(0, n - 1);
            if (w.v == 1) {
                int pos = w.seg.fr;
                auto it = alive.lower_bound({pos + 1, {-inf, -inf}});
                assert(it != alive.begin());
                it--;
                t.modify(it->fr, n - 1, -1);
                int id = it->sc.sc;
                dead.insert(*it);
                alive.erase(it);
                return id;
            }
            return -1;
        }

        // dead -> alive
        int del_val(int i) {
            i -= L;
            assert(i >= 0 && i < n);
            t.modify(i, n - 1, -1);
            auto w = t.ask(0, n - 1);
            int pos = 0;
            if (w.v == 0)
                pos = w.seg.sc + 1;
            auto it = dead.lower_bound({pos, {-inf, -inf}});
            if (it == dead.end())
                return -1;
            t.modify(it->fr, n - 1, 1);
            int id = it->sc.sc;
            alive.insert(*it);
            dead.erase(it);
            return id;
        }

        // who dies
        int add_segment(int i) {
            assert(i != -1);
            auto w = get_segment(i);
            alive.insert(w);
            int id = add_val(w.fr + L);
            return id;
        }

        // who lives
        int del_segment(int i) {
            assert(i != -1);
            auto w = get_segment(i);
            auto it = alive.find(w);
            if (it != alive.end()) {
                alive.erase(it);
                int id = del_val(w.fr + L);
                // you were in matching, return -1 or some other segment
                return id;
            } else {
                it = dead.find(w);
                assert(it != dead.end());
                dead.erase(it);
                // you weren't in matching, return yourself
                return i;
            }
        }
    };
    // Prefix expects real indices

    int n;

    // q[v] means P(right son)
    vector<Prefix> q, leafs;
    vector<set<int>> alive;

    void build(int v, int l, int r) {
        if (q.size() <= v) {
            q.resize(v + 1);
            alive.resize(v + 1);
        }
        if (l == r) {
            leafs[l].init(l, l);
            return;
        }
        int m = (l + r) / 2;
        build(2 * v, l, m);
        build(2 * v + 1, m + 1, r);
        q[v].init(m + 1, r);
    }

    void init(int n_) {
        arr.clear();
        q.clear();
        leafs.clear();
        alive.clear();
        segments.clear();
        n = n_;
        leafs.resize(n);
        arr.assign(n, 0);
        build(1, 0, n - 1);
    }

    // NOTE: in add_segment and del_segment a is always id, but for consistency with paper I name it a

    // a inserted to matched(v), b erased from matched(v)
    // if a = b then it means a not in matched(v)
    // so just push it upwards until you are left son
    pair<int, int> add_segment(int v, int l, int r, int id) {
        auto fix = [&](int a, int b) {
            if (a == b)
                return;
            assert(a != -1);
            alive[v].insert(a);
            if (b != -1)
                alive[v].erase(b);
        };
        if (l == r) {
            int del = leafs[l].add_segment(id);
            fix(id, del);
            return {id, del};
        }
        int m = (l + r) / 2;
        int st = segments[id].fr;
        if (st <= m) {
            auto [a, b] = add_segment(2 * v, l, m, id);
            if (b != -1 && segments[b].sc > m) {
                // transfer b to R
                int c = q[v].add_segment(b);
                b = c;
            }
            fix(a, b);
            return {a, b};
        } else {
            auto [a, b] = add_segment(2 * v + 1, m + 1, r, id);
            if (a == b) {
                // a may be in transfer(R), so push it up
                return {a, b};
            }
            if (b != -1 && alive[v].count(b)) {
                // b in matching(v) -> a in matching(v) instead of b
                // b stopped here before, now it should go up, so we return (a, b)
                int c1 = q[v].del_segment(b);
                int c = q[v].add_segment(a);
                assert(c == c1);
            } else {
                // b not in matching(v) -> it is in transfer/infeasible
                // anyway its position doesn't change (if it was in transfer it is still in transfer)
                // so we find new b
                if (b != -1) {
                    int tmp = q[v].del_segment(b);
                    assert(tmp == b);
                }
                b = q[v].add_segment(a);
            }
            fix(a, b);
            return {a, b};
        }
    }

    // a erased from matched(v), b inserted to matched(v)
    // if a = b then it means
    // 1) you are not in matched(v)
    // 2) just push a upwards until you are left son
    pair<int, int> del_segment(int v, int l, int r, int id) {
        auto fix = [&](int a, int b) {
            if (a == b)
                return;
            assert(a != -1);
            alive[v].erase(a);
            if (b != -1)
                alive[v].insert(b);
        };
        if (l == r) {
            int add = leafs[l].del_segment(id);
            fix(id, add);
            return {id, add};
        }
        int m = (l + r) / 2;
        int st = segments[id].fr;
        if (st <= m) {
            auto [a, b] = del_segment(2 * v, l, m, id);
            if (b != -1 && segments[b].sc > m) {
                // (transfer b to R) becomes (b in matched(L))
                int c = q[v].del_segment(b);
                b = c;
            }
            fix(a, b);
            return {a, b};
        } else {
            auto [a, b] = del_segment(2 * v + 1, m + 1, r, id);
            if (a == b) {
                // a not in matched(R)
                // but may be in transfer(R), so push it up
                return {a, b};
            }
            if (alive[v].count(a)) {
                // a in matched(v) -> someone (b or some better c) might get to matched(v)
                // note that b and c may be -1
                int c = q[v].del_segment(a);
                if (b != -1) {
                    int c1 = q[v].add_segment(b);
                    if (c1 == b) {
                        // b is worse than c
                        b = c;
                    } else {
                        // b is better than c
                        // so del(a) added c, add(b) killed c
                        assert(c1 == c);
                    }
                } else {
                    // b = -1, find new b
                    b = c;
                }
            } else {
                // a not in matched(v) -> b not in matched(v)
                // b remains in transfer/infeasible(v) (refer to add_segment, same else)
                // so return (a, a)
                int c = q[v].del_segment(a);
                assert(c == a);
                if (b != -1) {
                    int tmp = q[v].add_segment(b);
                    assert(tmp == b);
                }
                b = a;
            }
            fix(a, b);
            return {a, b};
        }
    }

    // a inserted to matched(v)
    int add_a(int v, int l, int r, int id) {
        auto fix = [&](int a) {
            if (a != -1)
                alive[v].insert(a);
        };
        if (l == r) {
            int a = leafs[l].del_val(id);
            fix(a);
            return a;
        }
        int m = (l + r) / 2;
        if (id <= m) {
            int a = add_a(2 * v, l, m, id);
            if (a != -1 && segments[a].sc > m) {
                // a in transfer(L) -> a in matched(L)
                int b = q[v].del_segment(a);
                if (b == a) {
                    // a wasn't in matched(v)
                } else {
                    // a was in matched(v)
                    // but b wasn't, return b
                    a = b;
                }
            }
            fix(a);
            return a;
        } else {
            int a = add_a(2 * v + 1, m + 1, r, id);
            int b = q[v].del_val(id);
            if (a == -1) {
                a = b;
            } else {
                int c = q[v].add_segment(a);
                if (c == a) {
                    // b is better than a
                    // so b now in matching(v)
                    a = b;
                } else {
                    assert(b == c);
                    // a now in matching(v)
                }
            }
            fix(a);
            return a;
        }
    }

    // a erased from matched(v)
    int del_a(int v, int l, int r, int id) {
        auto fix = [&](int a) {
            if (a != -1)
                alive[v].erase(a);
        };
        if (l == r) {
            int a = leafs[l].add_val(id);
            fix(a);
            return a;
        }
        int m = (l + r) / 2;
        if (id <= m) {
            int a = del_a(2 * v, l, m, id);
            if (a != -1 && segments[a].sc > m) {
                // a goes to transfer(L)
                a = q[v].add_segment(a);
            }
            fix(a);
            return a;
        } else {
            int a = del_a(2 * v + 1, m + 1, r, id);
            int b = q[v].add_val(id);
            if (a == -1) {
                a = b;
            } else {
                int c = q[v].del_segment(a);
                if (c == a) {
                    // id was matched with b <= a (perhaps b=a)
                    // a wasn't in matched(v), push b
                    a = b;
                } else {
                    // a was in matched(v), simply return a
                    assert(b == c);

                }
            }
            fix(a);
            return a;
        }
    }

    // returns index of new segment
    int add_segment(int l, int r) {
        assert(l >= 0 && l < n);
        assert(r >= 0 && r < n);
        assert(l <= r);
        int id = segments.size();
        segments.emplace_back(l, r);
        add_segment(1, 0, n - 1, id);
        return id;
    }

    void del_segment(int id) {
        assert(segments[id].fr <= segments[id].sc);
        del_segment(1, 0, n - 1, id);
        segments[id] = {-1, -2};
    }

    void add_a(int i) {
        assert(i >= 0 && i < n);
        add_a(1, 0, n - 1, i);
        arr[i]++;
    }

    void del_a(int i) {
        assert(i >= 0 && i < n);
        assert(arr[i] > 0);
        del_a(1, 0, n - 1, i);
        arr[i]--;
    }

    int get_matching() {
        return alive[1].size();
    }
}
