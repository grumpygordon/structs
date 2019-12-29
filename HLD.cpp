#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const int maxn = 1e6, lg = 20, mod = 1e9 + 7;

struct tree{
    int sum, all, u;
    tree *l, *r;
};

typedef tree * ptr;

int n, zap, parent[maxn], sz[maxn], sp[maxn], sparse[maxn][lg], p[maxn], pos[maxn];

int h[maxn];

vector<int> hldv[maxn];

ptr hld[maxn];

vector<int> sprs;

vector<int> e[maxn];

void dfs1(int v, int par){
    sp[v] = sprs.size();
    sprs.push_back(v);
    parent[v] = par;
    sz[v] = 1;
    if (par != -1)
        h[v] = h[par] + 1;
    for (int i = 0; i < e[v].size(); i++)
        if (e[v][i] != par)
            dfs1(e[v][i], v), sz[v] += sz[e[v][i]], sprs.push_back(v);
}

int findlca(int v, int u){
    if (sp[v] > sp[u])
        swap(v, u);
    int st = 0;
    while ((1 << (st + 1)) <= sp[u] - sp[v] + 1)
        st++;
    if (h[sparse[sp[v]][st]] < h[sparse[sp[u] - (1 << st) + 1][st]])
        return sparse[sp[v]][st];
    return sparse[sp[u] - (1 << st) + 1][st];
}

void make_sparse(){
    for (int i = 0; i < sprs.size(); i++)
        sparse[i][0] = sprs[i];
    for (int j = 1; (1 << j) <= sprs.size(); j++)
        for (int i = 0; i + (1 << j) <= sprs.size(); i++)
            if (h[sparse[i][j - 1]] < h[sparse[i + (1 << (j - 1))][j - 1]])
                sparse[i][j] = sparse[i][j - 1];
            else
                sparse[i][j] = sparse[i + (1 << (j - 1))][j - 1];
}

void dfs(int v, int par){
    p[v] = par;
    pos[v] = hldv[par].size();
    hldv[par].push_back(v);
    int ind, maxsz = -1;
    for (int i = 0; i < e[v].size(); i++)
        if (e[v][i] != parent[v] && sz[e[v][i]] > maxsz)
            maxsz = sz[e[v][i]], ind = e[v][i];
    for (int i = 0; i < e[v].size(); i++)
        if (e[v][i] != parent[v])
            if (ind == e[v][i])
                dfs(e[v][i], par);
            else
                dfs(e[v][i], e[v][i]);
}

ptr build(int l, int r, int ind){
    ptr t = new tree;
    t->sum = 0;
    t->all = r - l + 1;
    t->u = 0;
    if (l == r){
        t->l = nullptr;
        t->r = nullptr;
        return t;
    }
    int m = (l + r) / 2;
    t->l = build(l, m, ind);
    t->r = build(m + 1, r, ind);
    return t;
}

void push(ptr &t) {
    if (t == 0 || t->u == 0)
        return;
    t->sum = t->all - t->sum;
    if (t->l != 0)
        t->l->u ^= 1;
    if (t->r != 0)
        t->r->u ^= 1;
    t->u = 0;
}

int update(ptr &t, int tl, int tr, int l, int r){
    if (t == 0)
        return 0;
    push(t);
    if (l > r)
        return 0;
    if (tl == l && tr == r) {
        t->u = 1;
        push(t);
        return t->sum;
    }
    int m = (tl + tr) / 2;
    int res = update(t->l, tl, m, l, min(r, m)) +
    update(t->r, m + 1, tr, max(l, m + 1), r);
    t->sum = t->l->sum + t->r->sum;
    return res;
}

void precalc(){
    dfs1(0, -1);
    parent[0] = 0;
    make_sparse();
    dfs(0, 0);
    for (int i = 0; i < n; i++)
        if (hldv[i].size() > 0)
            hld[i] = build(0, (int)hldv[i].size() - 1, i);
}

struct pth{
    int i, l, r;
};

pth make_pth(int i, int l, int r){
    pth ret;
    ret.i = i;
    ret.l = l;
    ret.r = r;
    return ret;
}

vector<pth> findpath(int v){
    vector<pth> ret;
    while (1){
        ret.push_back(make_pth(p[v], 0, pos[v]));
        if (p[v] == 0)
            break;
        v = parent[p[v]];
    }
    return ret;
}

int main(){
    #ifdef ONPC
    freopen("a.in", "r", stdin);
    freopen("a.out", "w", stdout);
    #endif // ONPC
    ios::sync_with_stdio(0);
    cin.tie(0);
    cin >> n;
    for (int i = 1; i < n; i++){
        int v, u;
        cin >> v >> u;
        v--;
        u--;
        e[v].push_back(u);
        e[u].push_back(v);
    }
    precalc();
    int ans = 0;
    int zap;
    cin >> zap;
    for (int it = 0; it < zap; it++){
        int v;
        cin >> v;
        v--;
        vector<pth> path = findpath(v);
        for (int i = 0; i < path.size(); i++)
             ans += 2 * update(hld[path[i].i], 0, (int)hldv[path[i].i].size() - 1, path[i].l, path[i].r);
        ans -= h[v] + 1;
        cin >> v;
        v--;
        path = findpath(v);
        for (int i = 0; i < path.size(); i++)
             ans += 2 * update(hld[path[i].i], 0, (int)hldv[path[i].i].size() - 1, path[i].l, path[i].r);
        ans -= h[v] + 1;
        cout << ans << '\n';
    }
}