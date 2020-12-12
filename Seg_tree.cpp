
struct Val {
    ll v;
    Val() : v(0) {}
    Val(ll x) : v(x) {}

    friend Val operator+(Val a, Val b);

    Val& operator+=(Val a) {
        return *this = *this + a;
    }
};

Val operator+(Val a, Val b) {
    return Val(a.v + b.v);
}

bool operator==(Val a, Val b) {
    return a.v == b.v;
}

bool operator!=(Val a, Val b) {
    return !(a == b);
}

struct Segtree {

    static Val calc(Val a, Val b) {
        return min(a.v, b.v);
    }

    Val q[4 * maxn], up[4 * maxn];
    int n;

    void build(int v, int l, int r) {
        q[v] = llinf;
        up[v] = 0;
        if (l == r) {
            return;
        }
        int m = (l + r) / 2;
        build(2 * v, l, m);
        build(2 * v + 1, m + 1, r);
    }
    void build(int n_) {
        n = n_;
        build(1, 0, n - 1);
    }

    void push(int v, int l, int r) {
        if (up[v] != Val(0)) {
            q[v] += up[v];
            if (l != r) {
                up[2 * v] += up[v];
                up[2 * v + 1] += up[v];
            }
            up[v] = 0;
        }
    }

    void update(int v, int tl, int tr, int l, int r, Val w) {
        push(v, tl, tr);
        if (l > r)
            return;
        if (tl == l && tr == r) {
            up[v] += w;
            push(v, tl, tr);
            return;
        }
        int m = (tl + tr) / 2;
        update(2 * v, tl, m, l, min(r, m), w);
        update(2 * v + 1, m + 1, tr, max(l, m + 1), r, w);
        q[v] = calc(q[2 * v], q[2 * v + 1]);
    }

    void update(int l, int r, ll w) {
        update(1, 0, n - 1, l, r, w);
    }

    Val get(int v, int tl, int tr, int l, int r) {
        push(v, tl, tr);
        if (l > r)
            return llinf;
        if (tl == l && tr == r)
            return q[v];
        int m = (tl + tr) / 2;
        return calc(get(2 * v, tl, m, l, min(r, m)),
                   get(2 * v + 1, m + 1, tr, max(l, m + 1), r));
    }

    Val get(int l, int r) {
        return get(1, 0, n - 1, l, r);
    }

};
