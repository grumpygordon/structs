struct cht {
    vector<pair<int, ll> > q;
    vector<pair<ll, ll> > p;
    void add(int k, ll b) {
        q.push_back({k, b});
    }

    pair<ll, ll> inter(pair<int, ll> a, pair<int, ll> b) {
        return {b.sc - a.sc, a.fr - b.fr};
    }

    bool more(pair<ll, ll> a, pair<ll, ll> b) {
        return a.fr / (ld)a.sc > b.fr / (ld)b.sc;
    }

    bool moeq(pair<ll, ll> a, pair<ll, ll> b) {
        return a.fr / (ld)a.sc >= b.fr / (ld)b.sc;
    }

    void prec() {
        sort(q.begin(), q.end());
        vector<pair<int, ll> > g;
        for (auto it : q) {
            if (g.empty()) {
                g.push_back(it);
                continue;
            }
            while (!g.empty()) {
                if (g.back().fr == it.fr || (g.size() > 1 && moeq(p.back(), inter(g.back(), it)))) {
                    g.pop_back();
                    if (!g.empty())
                        p.pop_back();
                } else
                    break;
            }
            if (!g.empty())
                p.push_back(inter(g.back(), it));
            g.push_back(it);
        }
        swap(q, g);
    }

    ll get(int w) {
        if (q.empty())
            return 0;
        int l = -1, r = q.size() - 1;
        while (r - l > 1) {
            int m = (l + r) / 2;
            if (more({w, 1}, p[m]))
                l = m;
            else
                r = m;
        }
        return q[r].fr * (ll)w + q[r].sc;
    }
};