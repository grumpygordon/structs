// small to large on tree O(nlogn)
#include <bits/stdc++.h>
 
const int N = 100010, L = 20;
 
using Info = std::pair<int, int>;
 
const Info BAD(0, 0);
 
struct Node {
    Info in, de;
    int ls, rs;
} P[N * L];
 
int top;
void clear() {
    top = 0;
    memset(P, 0, sizeof(P));
}
int clone(int o) {
    int p = ++top;
    memcpy(P + p, P + o, sizeof(Node));
    return p;
}
void upd(int o) {
    P[o].in = std::max(P[P[o].ls].in, P[P[o].rs].in);
    P[o].de = std::max(P[P[o].ls].de, P[P[o].rs].de);
}
Info qin(int o, int l, int r, int k) {
    if (!o || k < l) return BAD;
    if (r <= k) return P[o].in;
    int mid = (l + r) / 2;
    return std::max(qin(P[o].ls, l, mid, k), qin(P[o].rs, mid + 1, r, k));
}
Info qde(int o, int l, int r, int k) {
    if (!o || k > r) return BAD;
    if (k <= l) return P[o].de;
    int mid = (l + r) / 2;
    return std::max(qde(P[o].ls, l, mid, k), qde(P[o].rs, mid + 1, r, k));
}
int ins(int o, int l, int r, int k, const Info& in, const Info& de) {
    o = clone(o);
    if (l == r) {
        P[o].in = in;
        P[o].de = de;
        return o;
    }
    int mid = (l + r) / 2;
    if (k <= mid) P[o].ls = ins(P[o].ls, l, mid, k, in, de);
    else P[o].rs = ins(P[o].rs, mid + 1, r, k, in, de);
    upd(o);
    return o;
}
 
int n;
std::vector<int> g[N];
int a[N];
 
struct Solve {
    int root, curu;
    Info inc[N], dec[N];
    int sa[N];
    struct {
        int len, lca, x, y;
    } diam;
 
    void test(Info in, Info de, int lca) {
        int len = in.first + de.first + (lca != 0);
        sa[curu] = std::max(sa[curu], len);
        if (diam.len < len)
            diam = {len, lca, in.second, de.second};
    }
 
    int merge(int p, int q, int l, int r) {
        if (!p || !q) return p ^ q;
        if (l == r) {
            P[p].in = std::max(P[p].in, P[q].in);
            P[p].de = std::max(P[p].de, P[q].de);
            return p;
        }
        test(P[P[p].ls].in, P[P[q].rs].de, 0);
        test(P[P[q].ls].in, P[P[p].rs].de, 0);
        int mid = (l + r) / 2;
        P[p].ls = merge(P[p].ls, P[q].ls, l, mid);
        P[p].rs = merge(P[p].rs, P[q].rs, mid + 1, r);
        upd(p);
        return p;
    }
 
    int dfs(int u, int p) {
        int rt = 0;
        Info iin = BAD, dde = BAD;
        {
            curu = u; test(BAD, BAD, u);
        }
        for (int v : g[u]) if (v != p) {
            int sub = dfs(v, u);
            sa[u] = std::max(sa[u], sa[v]);
            Info rin = qin(rt, 1, n, a[u] - 1), rde = qde(rt, 1, n, a[u] + 1),
                 sin = qin(sub, 1, n, a[u] - 1), sde = qde(sub, 1, n, a[u] + 1);
            curu = u;
            test(rin, sde, u);
            test(sin, rde, u);
            iin = std::max(iin, sin);
            dde = std::max(dde, sde);
            rt = merge(rt, sub, 1, n);
        }
        ++iin.first; inc[u] = iin;
        ++dde.first; dec[u] = dde;
        iin.second = dde.second = u;
        rt = ins(rt, 1, n, a[u], iin, dde);
//      std::cerr << root << " -> " << u << " = " << sa[u] << "\n";
        return rt;
    }
 
    void main() {
        clear();
        dfs(root, -1);
//      std::cerr << diam.len << ", " << diam.x << ' ' << diam.y << ' ' << diam.lca << '\n';
    }
} fir, le, ri;
 
int prt[N];
void dfs(int u) {
    for (int v : g[u]) if (v != prt[u]) {
        prt[v] = u;
        dfs(v);
    }
}
 
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
 
    std::cin >> n;
    for (int rep = 1; rep != n; ++rep) {
        int u, v; std::cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for (int i = 1; i <= n; ++i) std::cin >> a[i];
    fir.root = 1;
    fir.main();
/*  if (fir.diam.len == 1) {
        std::cout << "1\n";
        return 0;
    }*/
    int x = fir.diam.x, y = fir.diam.y;
    if (!x) x = fir.diam.lca;
    else while (fir.inc[x].second) x = fir.inc[x].second;
    if (!y) y = fir.diam.lca;
    else while (fir.dec[y].second) y = fir.dec[y].second;
 
    le.root = x; ri.root = y;
//  std::cerr << "From " << x << " to " << y << '\n';
    le.main(); ri.main();
 
    prt[x] = -1; dfs(x);
    int ans = fir.diam.len;
    for (int v = y; v != -1; v = prt[v]) {
        int mx = 0;
        for (int u : g[v]) {
            if (u == prt[v]) mx = std::max(mx, ri.sa[u]);
            else mx = std::max(mx, le.sa[u]);
        }
        ans = std::min(ans, mx);
    }
    std::cout << ans << '\n';
 
    return 0;
}