#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

const int maxn = 2e5 + 100, inf = 1e9 + 100;

int n, m;

vector<int> comp[maxn];

int col[maxn];

vector<pair<int, int> > e;

unordered_set<int> E[maxn];

bool vis[maxn];

void dfs(int v, int x){
    col[v] = x;
    vis[v] = 1;
    for (auto i : E[v])
        if (!vis[i])
            dfs(i, x);
}

int main(){
    srand(time(nullptr));
    ios::sync_with_stdio(0);
    n = 50, m = 10000;
    for (int i = 0; i < n; i++)
        col[i] = i;
    int x = n;
    vector<pair<string, pair<int, int> > > ans;
    for (int iter = 0; iter < m; iter++){
        int t = rand() % 2;
        if (t == 0){
            vector<int> p;
            unordered_set<int> q;
            for (int j = 0; j < n; j++)
                if (q.count(col[j]) == 0)
                    q.insert(col[j]), p.push_back(j);
            if (p.size() == 1){
                continue;
            }
            while (1){
                int x = rand() % p.size(), y = rand() % p.size();
                if (x == y)
                    continue;
                ans.push_back(make_pair("link ", make_pair(p[x] + 1, p[y] + 1);
                e.push_back(make_pair(p[x], p[y]));
                for (int i = 0; i < n; i++)
                    if (col[i] == col[p[y]] && i != p[y])
                        col[i] = col[p[x]];
                E[p[x]].insert(p[y]);
                E[p[y]].insert(p[x]);
                col[p[y]] = col[p[x]];
                break;
            }
        }
        else
        if (t == 1){
            if (e.empty()){
                continue;
            }
            int i = rand() % e.size();
            ans.push_back(make_pair("cut ", make_pair(e[i].first + 1, e[i].second + 1)));
            E[e[i].first].erase(e[i].second);
            E[e[i].second].erase(e[i].first);
            for (int i = 0; i < n; i++)
                vis[i] = 0;
            dfs(e[i].second, x);
            x++;
            e.erase(e.begin() + i);
        }
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                ans.push_back(make_pair("get ", make_pair(i + 1, j + 1)));
    }
    cout << n << ' ' << ans.size() << '\n';
    for (int i = 0; i < ans.size(); i++)
        cout << ans[i].first << ans[i].second.first << ' ' << ans[i].second.second << '\n';
}
