/*
         o###########oo
      o##"          ""##o
    o#"                "##
  o#"                    "#o
 #"  ##              ##   "##
#"                          ##
#  ###################       #
#                            #
#                            #
#                            #
#                            #
#                            #
#                            #
#o                           #
"#o                         ##
 "#o                       ##
  "#o                    o#"
   "#o                  ##
     "#o              o#"
       "#ooo      ooo#######oo
        ###############   "######o
     o###""        "###o      # ###
   o###o     oooo    ###    oo####"
 o###**#     #**#   ############"
 ""##""""""""""###########    #
    # oooooooo#"#**     ##    #
    # #       # # **    ##    #
    #o#       #o#  *****###ooo#
                        ##
                        ##   o###o
                        ## o##***##
               o########## #***#**##o
             o##"   ""###  #***##***#
 o#######o  ###   oo####   ##**####*#
o##"  ""#############""     ##****###
##"         ##              ##*##*###
##          ###              ##### ##
##           ###              # ##  #
##            ##                 #
##             ##
##             ###
##              ###oo
###              ""###
 ###
  ###
*/

#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

typedef unsigned long long ull;

typedef long double ld;

ll AR = 19, BR = 13, CR = 23, XR = 228, YR = 322, MOD = 1e9 + 993;

ll myrand()
{
    ll ZR = (XR * AR + YR * BR + CR);
    XR = YR;
    YR = ZR;
    return ZR;
}

inline int sqr(int x)
{
    return x * x;
}

int md(int x, int mymod)
{
    if (x >= mymod)
        x -= mymod;
    if (x < 0)
        x += mymod;
    return x;
}

int gcd(int x, int y)
{
    if (x == 0 || y == 0)
        return max(1, max(x, y));
    return __gcd(x, y);
}

ll lcm(ll x, ll y)
{
    return x * y / gcd(x, y);
}

pair<int, int> compress(pair<int, int> x)
{
    if (x.second < 0)
        x.first *= -1, x.second *= -1;
    int g = gcd(abs(x.first), x.second);
    return make_pair(x.first / g, x.second / g);
}

pair<int, int> mkp(int x, int y)
{
    return make_pair(x, y);
}

int mystoi(string &s)
{
    int ret = 0;
    int vl = 1;
    if (s[0] == '-')
        vl = -1;
    for (int i = (vl == 1 ? 0 : 1); i < s.length(); i++)
        ret = ret * 10 + s[i] - 48;
    return ret * vl;
}

pair<int, int> operator +(pair<int, int> X, pair<int, int> Y)
{
    return compress(make_pair(X.first * Y.second + X.second * Y.first, X.second * Y.second));
}

pair<int, int> operator -(pair<int, int> X, pair<int, int> Y)
{
    return compress(make_pair(X.first * Y.second - X.second * Y.first, X.second * Y.second));
}

pair<int, int> operator *(pair<int, int> X, pair<int, int> Y)
{
    return compress(make_pair(X.first * Y.first, X.second * Y.second));
}

pair<int, int> operator /(pair<int, int> X, pair<int, int> Y)
{
    return compress(make_pair(X.first * Y.second, X.second * Y.first));
}

const ll llinf = 2e18 + 100;

const ld EPS = 1e-6;

const ld PI = atan2(0, -1);

const int maxn = 2e5 + 100, inf = 1e9 + 100, sq = 300, mod = 1e9 + 993, LEN = 26;

void minAKA(vector<vector<int> > &e, vector<bool> &ends, int &start, int L = LEN)
{
    vector<vector<vector<int> > > eo;
    int n = e.size();
    eo.resize(n, vector<vector<int> >(L));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < L; j++)
            eo[e[i][j]][j].push_back(i);
    vector<int> col(n);
    vector<unordered_set<int> > q(2);
    for (int i = 0; i < n; i++)
        col[i] = ends[i], q[col[i]].insert(i);
    if (q[1].empty())
        q.pop_back();
    if (q[0].empty())
        swap(q[0], q[1]), q.pop_back();
    queue<int> que;
    for (int i = 0; i < q.size(); i++)
        que.push(i);
    while (!que.empty())
    {
        int C = que.front();
        que.pop();
        for (int x = 0; x < L; x++)
        {
            unordered_map<int, unordered_set<int> > inv;
            for (auto i : q[C])
            {
                for (int j = 0; j < eo[i][x].size(); j++)
                {
                    int to = eo[i][x][j];
                    inv[col[to]].insert(to);
                }
            }
            for (unordered_map<int, unordered_set<int> > :: iterator it = inv.begin(); it != inv.end(); it++)
            {
                int cnow = it->first;
                if (q[cnow].size() == it->second.size())
                    continue;
                unordered_set<int> r1;
                swap(r1, it->second);
                int cr1 = q.size();
                for (auto ij : r1)
                    q[cnow].erase(ij), col[ij] = cr1;
                if (q[cnow].size() < r1.size())
                {
                    swap(q[cnow], r1);
                    for (auto ij : q[cnow])
                        col[ij] = cnow;
                    for (auto ij : r1)
                        col[ij] = cr1;
                }
                q.push_back(r1);
                que.push(cr1);
            }
        }
    }
    int newn = q.size();
    vector<vector<int> > er(newn, vector<int>(L));
    vector<bool> endsr(newn, 0);
    vector<bool> vis(q.size(), 0);
    for (int i = 0; i < n; i++)
        if (!vis[col[i]])
        {
            for (int j = 0; j < L; j++)
                er[col[i]][j] = col[e[i][j]];
            endsr[col[i]] = ends[i];
            vis[col[i]] = 1;
        }
    e = er;
    ends = endsr;
    start = col[0];
}

int n, m, k;

int e[maxn][LEN];

vector<pair<int, int> > eo[maxn];

bool nd[maxn];

bool vis[maxn];

int d[maxn];

int ind[maxn];

void dfs(int v)
{
    vis[v] = 1;
    d[v]++;
    for (int i = 0; i < LEN; i++)
    {
        int to = e[v][i];
        if (!vis[to])
            dfs(to);
    }
}

int main()
{
#ifdef ONPC
    //ifstream cin("a.in");
    //ofstream cout("a.out");
    freopen("a.in", "r", stdin);
    freopen("a.out", "w", stdout);
#else
    //ifstream cin("input.txt");
    //ofstream cout("output.txt");
    freopen("fastminimization.in", "r", stdin);
    freopen("fastminimization.out", "w", stdout);
#endif // ONPC
    //ios::sync_with_stdio(0);
    //cin.tie(0);
    //cout.tie(0);
    scanf("%d %d %d", &n, &m, &k);
    vector<vector<int> > E(n + 1, vector<int>(LEN, -1));
    vector<bool> det(n + 1, 0);
    for (int i = 0; i < k; i++)
    {
        int x;
        scanf("%d", &x);
        det[x - 1] = 1;
    }
    for (int i = 0; i < m; i++)
    {
        int a, b;
        char w;
        scanf("%d %d %c", &a, &b, &w);
        E[a - 1][w - 'a'] = b - 1;
    }
    for (int i = 0; i <= n; i++)
        for (int j = 0; j < LEN; j++)
            if (E[i][j] == -1)
                E[i][j] = n;
    n++;
    int start;
    minAKA(E, det, start);
    n = E.size();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < LEN; j++)
            e[i][j] = E[i][j], eo[e[i][j]].push_back(make_pair(i, j));
    for (int i = 0; i < n; i++)
        nd[i] = det[i];
    queue<int> que;
    for (int i = 0; i < n; i++)
        if (nd[i])
            vis[i] = 1, que.push(i), d[i] = 1;
    d[start] = 1;
    while (!que.empty())
    {
        int v = que.front();
        que.pop();
        for (int i = 0; i < eo[v].size(); i++)
        {
            int to = eo[v][i].first;
            if (!vis[to])
            {
                vis[to] = 1;
                d[to] = 1;
                que.push(to);
            }
        }
    }
    memset(vis, 0, sizeof(vis));
    dfs(start);
    ind[start] = 0;
    int sz = 1;
    for (int i = 0; i < n; i++)
        if (i != start)
            if (d[i] == 2)
                ind[i] = sz, sz++;
            else
                ind[i] = -1;
    m = 0;
    k = 0;
    vector<pair<pair<int, int>, char> > edge;
    vector<int> term;
    for (int i = 0; i < n; i++)
        if (ind[i] != -1)
        {
            if (nd[i])
                term.push_back(ind[i] + 1), k++;
            for (int j = 0; j < LEN; j++)
            {
                int to = e[i][j];
                if (ind[to] != -1)
                    edge.push_back(make_pair(make_pair(ind[i] + 1, ind[to] + 1), j + 'a')), m++;
            }
        }
    n = sz;
    printf("%d %d %d\n", n, m, k);
    for (int i = 0; i < k; i++)
        printf("%d ", term[i]);
    printf("\n");
    for (int i = 0; i < m; i++)
        printf("%d %d %c\n", edge[i].first.first, edge[i].first.second, edge[i].second);
}
