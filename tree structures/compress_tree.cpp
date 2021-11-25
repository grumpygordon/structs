//Solver was HLD, use anything else: init(n), add_edge(v->u), precalc()
//take lca from your file in github
struct Virt_tree {
    map<int, int> mp;

    Solver s;

    void init(vector<int> who) {
        if (who.empty())
            return;
        sort(all(who), [&](int i, int j) {return seg[i].fr < seg[j].fr;});
        unique(who);
        int sz = who.size();
        for (int i = 0; i + 1 < sz; i++)
            who.push_back(get_lca(who[i], who[i + 1]));
        sort(all(who), [&](int i, int j) {return seg[i].fr < seg[j].fr;});
        unique(who);
        for (int i = 0; i < who.size(); i++)
            mp[who[i]] = i;
        s.init(who.size());
        vector<int> bu;
        for (int i = 0; i < who.size(); i++) {
            if (bu.empty()) {
                bu.push_back(i);
                continue;
            }
            while (!bu.empty()) {
                int v = who[bu.back()];
                if (seg[v].fr <= seg[who[i]].fr && seg[v].sc >= seg[who[i]].sc)
                    break;
                bu.pop_back();
            }
            assert(!bu.empty());
            s.add_edge(bu.back(), i);
            bu.push_back(i);
        }
        s.precalc();
    }

    int get_id(int v) {
        return mp[v];
    }

    void upd(int v, int w) {
        s.upd(get_id(v), w);
    }

    int calc(int v, int u) {
        int lca = get_lca(v, u);
        return s.calc(get_id(v), get_id(u), get_id(lca));
    }

};
