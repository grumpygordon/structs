//find k smallest paths from s to t in digraph
//O(nlogn+klogk)

struct X {
    ll w;
    int to;

};
bool operator<(X a, X b) {
    return a.w < b.w;
}

struct heap {
    X w;
    int h;
    heap *l, *r;
    heap(X w_) {
        w = w_;
        l = r = 0;
        h = 1;
    }
};

typedef heap* ptr;

ptr create(X w) {
    return new heap(w);
}

ptr merge(ptr t1, ptr t2) {
    if (t1 == 0 && t2 == 0)
        return 0;
    if (t1 == 0)
        return t2;
    if (t2 == 0)
        return t1;
    if (t2->w < t1->w)
        swap(t1, t2);
    ptr z = new heap(*t1);
    z->r = merge(t1->r, t2);
    if (z->l == 0 || z->l->h < z->r->h)
        swap(z->l, z->r);
    z->h = 1 + (z->r == 0 ? 0 : z->r->h);
    return z;
}

ptr pop(ptr t) {
    if (t != 0)
        return merge(t->l, t->r);
    return 0;
}

ptr push(ptr t, X w) {
    return merge(t, create(w));
}

struct edge {
    int to, w, i;
};

vector<ll> solve(vector<vector<pair<int, int>>> e_, int k, int s = 0, int t = -1) {
    // (to, w)
    int n = e_.size();
    if (t == -1)
        t = n - 1;
    vector<vector<edge>> e(n);
    {
        int cnt = 0;
        for (int i = 0; i < n; i++)
            for (auto j : e_[i])
                e[i].push_back({j.fr, j.sc, cnt++});
    }
    vector<vector<edge>> eo(n);
    for (int i = 0; i < n; i++)
        for (auto j : e[i])
            eo[j.to].push_back({i, j.w, j.i});
    vector<ll> ds(n, llinf);
    vector<int> par(n, -1);
    vector<int> prr(n, -1);
    {
        ds[t] = 0;
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> que;
        que.push({0, t});
        while (!que.empty()) {
            auto [w, v] = que.top();
            que.pop();
            if (w != ds[v])
                continue;
            for (auto i : eo[v])
                if (ds[i.to] > ds[v] + i.w) {
                    ds[i.to] = ds[v] + i.w;
                    par[i.to] = i.i;
                    prr[i.to] = v;
                    que.push({ds[i.to], i.to});
                }
        }
    }
    vector<vector<int>> ces(n);
    for (int i = 0; i < n; i++)
        if (prr[i] != -1)
            ces[prr[i]].push_back(i);
    vector<ptr> g(n);
    {
        vector<int> que;
        que.push_back(t);
        for (int it = 0; it < que.size(); it++) {
            int v = que[it];
            for (int i : ces[v])
                que.push_back(i);
            if (ds[v] != llinf) {
                g[v] = 0;
                for (auto i : e[v])
                    if (ds[i.to] != llinf) {
                        if (i.i == par[v])
                            g[v] = merge(g[v], g[i.to]);
                        else
                            g[v] = push(g[v], X{i.w - ds[v] + ds[i.to], i.to});
                    }
            }
        }
    }
    vector<ll> ans;
    priority_queue<pair<ll, ptr>, vector<pair<ll, ptr>>, greater<pair<ll, ptr>>> que;
    if (ds[s] != llinf)
        que.push({ds[s], create({ds[s], s})});
    while (!que.empty() && ans.size() < k) {
        auto [w, h] = que.top();
        que.pop();
        ans.push_back(w);
        int v = h->w.to;
        if (g[v] != 0)
            que.push({w + g[v]->w.w, g[v]});

        if (h->l != 0)
            que.push({w + h->l->w.w - h->w.w, h->l});
        if (h->r != 0)
            que.push({w + h->r->w.w - h->w.w, h->r});
    }
    while (ans.size() < k)
        ans.push_back(-1);
    return ans;
}
