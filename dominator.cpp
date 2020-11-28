
namespace Dominator {
// source: https://tanujkhattar.wordpress.com/2016/01/11/dominator-tree-of-a-directed-graph
// tree[v] - dominator tree
// N > n
typedef pair<int,int>   II;
typedef vector< II >      VII;
typedef vector<int>     VI;
typedef vector< VI > 	VVI;
typedef long long int 	LL;

#define PB push_back
#define MP make_pair
#define F first
#define S second
#define SZ(a) (int)(a.size())
#define ALL(a) a.begin(),a.end()
#define SET(a,b) memset(a,b,sizeof(a))
#define N maxn
VI g[N];
VI tree[N],rg[N],bucket[N];
int sdom[N],par[N],dom[N],dsu[N],label[N];
int arr[N],rev[N],T;
int Find(int u,int x=0)
{
    if(u==dsu[u])return x?-1:u;
    int v = Find(dsu[u],x+1);
    if(v<0)return u;
    if(sdom[label[dsu[u]]] < sdom[label[u]])
        label[u] = label[dsu[u]];
    dsu[u] = v;
    return x?v:label[u];
}
void Union(int u,int v) //Add an edge u-->v
{
    dsu[v]=u; 	//yup,its correct :)
}
void dfs0(int u)
{
    //cerr << u << '\n';
    T++;arr[u]=T;rev[T]=u;
    label[T]=T;sdom[T]=T;dsu[T]=T;
    for(int i=0;i<SZ(g[u]);i++)
    {
        int w = g[u][i];
        if(!arr[w])dfs0(w),par[arr[w]]=arr[u];
        rg[arr[w]].PB(arr[u]);
    }
}

void calculate(vector<vector<int>> edges, int root) {
    int n = edges.size();
    for (int i = 0; i < n; i++)
        g[i] = edges[i];
    //Build Dominator tree
    dfs0(root);
    n = T;
    for (int i = n; i >= 1; i--) {
        for (int j = 0; j < SZ(rg[i]); j++)
            sdom[i] = min(sdom[i], sdom[Find(rg[i][j])]);
        if (i > 1)bucket[sdom[i]].PB(i);
        for (int j = 0; j < SZ(bucket[i]); j++) {
            int w = bucket[i][j];
            int v = Find(w);
            if (sdom[v] == sdom[w])dom[w] = sdom[w];
            else dom[w] = v;
        }
        if (i > 1)Union(par[i], i);
    }
    for (int i = 2; i <= n; i++) {
        if (dom[i] != sdom[i])
            dom[i] = dom[dom[i]];
        tree[rev[i]].PB(rev[dom[i]]);
        tree[rev[dom[i]]].PB(rev[i]);
    }
}
    typedef pair<int,int>   II;
    typedef vector< II >      VII;
    typedef vector<int>     VI;
    typedef vector< VI > 	VVI;
    typedef long long int 	LL;

#undef PB
#undef MP
#undef F
#undef S
#undef SZ
#undef ALL
#undef SET
#undef N
}
