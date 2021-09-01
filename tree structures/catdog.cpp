// catdog from JOI 2018


struct Node {
    int c[2][2] = {{0, inf}, {inf, 0}};
};
 
struct Str {
    vector<Node> q;
 
    int s;
 
    void update(int v) {
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++) {
                q[v].c[i][j] = inf;
                for (int a = 0; a < 2; a++)
                    for (int b = 0; b < 2; b++) {
                        int x = q[2 * v].c[i][a], y = q[2 * v + 1].c[b][j];
                        if (x != inf && y != inf)
                            setmin(q[v].c[i][j], x + y + (a ^ b));
                    }
            }
    }
 
    void upd(int v, Node const &w) {
        //cerr << "before " << s << '\n';
        assert(v < s);
        v += s;
        q[v] = w;
        v /= 2;
        while (v) {
            update(v);
            v /= 2;
        }
        //cerr << "after " << s << '\n';
    }
 
    void init(int n) {
        s = 1;
        while (s < n)
            s *= 2;
        q.resize(2 * s);
        for (int i = s - 1; i > 0; i--)
            update(i);
    }
 
    pair<int, int> get() {
        pair<int, int> v = {min(q[1].c[0][0], q[1].c[0][1]),
                min(q[1].c[1][0], q[1].c[1][1])};
        return {min(v.fr, v.sc + 1), min(v.sc, v.fr + 1)};
    }
};
 
int n;
 
vector<int> e[maxn];
 
int p[maxn];
 
int sz[maxn];
 
int mas[maxn][2];
 
int who[maxn];
 
void prec(int v, int par) {
    p[v] = par;
    sz[v] = 1;
    if (v)
        e[v].erase(find(all(e[v]), par));
    for (int i : e[v])
        if (i != par) {
            prec(i, v);
            sz[v] += sz[i];
        }
    int id = 0;
    for (int i = 1; i < e[v].size(); i++)
        if (sz[e[v][id]] < sz[e[v][i]])
            id = i;
    swap(e[v][0], e[v][id]);
}
 
Str tree[maxn];
 
int hid[maxn], hpar[maxn], hsz[maxn];
 
void upd(int v) {
    Node s;
    s.c[0][0] = mas[v][0];
    s.c[1][1] = mas[v][1];
    if (who[v] == 0)
        s.c[1][1] = inf;
    else if (who[v] == 1)
        s.c[0][0] = inf;
    tree[hpar[v]].upd(hid[v], s);
}
 
void dfs(int v) {
    hid[v] = hsz[hpar[v]]++;
    //cerr << v << ' ' << hid[v] << ' ' << hpar[v] << '\n';
    if (e[v].empty()) {
        tree[hpar[v]].init(hsz[hpar[v]]);
        return;
    }
    hpar[e[v][0]] = hpar[v];
    dfs(e[v][0]);
    for (int i = 1; i < e[v].size(); i++) {
        hpar[e[v][i]] = e[v][i];
        dfs(e[v][i]);
    }
}
 
void solve() {
    cin >> n;
    for (int i = 0; i < n; i++)
        who[i] = -1;
    for (int i = 1; i < n; i++) {
        int v, u;
        cin >> v >> u;
        v--;
        u--;
        e[v].push_back(u);
        e[u].push_back(v);
    }
    prec(0, 0);
    dfs(0);
    int zaps;
    cin >> zaps;
    while (zaps--) {
        int tp, v;
        cin >> tp >> v;
        v--;
        who[v] = tp - 1;
        while (1) {
            int u = hpar[v];
            auto old = tree[u].get();
            upd(v);
            if (u == 0)
                break;
            auto res = tree[u].get();
            u = p[u];
            mas[u][0] += res.fr - old.fr;
            mas[u][1] += res.sc - old.sc;
            v = u;
        }
 
        auto res = tree[0].get();
        cout << min(res.fr, res.sc) << '\n';
    }
    exit(0);
}