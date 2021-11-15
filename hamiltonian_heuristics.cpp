//https://codeforces.com/blog/entry/90513

namespace hamil {
    template <typename T> bool chkmax(T &x,T y){return x<y?x=y,true:false;}
    template <typename T> bool chkmin(T &x,T y){return x>y?x=y,true:false;}
    #define vi vector<int>
    #define pb push_back
    #define mp make_pair
    #define pi pair<int, int>
    #define fi first
    #define se second
    #define ll long long
    namespace LCT {
        vector<vi> ch;
        vi fa, rev;
        void init(int n) {
            ch.resize(n + 1);
            fa.resize(n + 1);
            rev.resize(n + 1);
            for (int i = 0; i <= n; i++)
                ch[i].resize(2), 
                ch[i][0] = ch[i][1] = fa[i] = rev[i] = 0;
        }
        bool isr(int a)
        {
            return !(ch[fa[a]][0] == a || ch[fa[a]][1] == a);
        } 
        void pushdown(int a)
        {
            if(rev[a])
            {
                rev[ch[a][0]] ^= 1, rev[ch[a][1]] ^= 1;
                swap(ch[a][0], ch[a][1]);
                rev[a] = 0;
            }
        }
        void push(int a)
        {
            if(!isr(a)) push(fa[a]);
            pushdown(a); 
        }
        void rotate(int a)
        {
            int f = fa[a], gf = fa[f];
            int tp = ch[f][1] == a;
            int son = ch[a][tp ^ 1];
            if(!isr(f)) 
                ch[gf][ch[gf][1] == f] = a;    
            fa[a] = gf;

            ch[f][tp] = son;
            if(son) fa[son] = f;

            ch[a][tp ^ 1] = f, fa[f] = a;
        }
        void splay(int a)
        {
            push(a);
            while(!isr(a))
            {
                int f = fa[a], gf = fa[f];
                if(isr(f)) rotate(a);
                else
                {
                    int t1 = ch[gf][1] == f, t2 = ch[f][1] == a;
                    if(t1 == t2) rotate(f), rotate(a);
                    else rotate(a), rotate(a);    
                } 
            } 
        }
        void access(int a)
        {
            int pr = a;
            splay(a);
            ch[a][1] = 0;
            while(1)
            {
                if(!fa[a]) break; 
                int u = fa[a];
                splay(u);
                ch[u][1] = a;
                a = u;
            }
            splay(pr);
        }
        void makeroot(int a)
        {
            access(a);
            rev[a] ^= 1;
        }
        void link(int a, int b)
        {
            makeroot(a);
            fa[a] = b;
        }
        void cut(int a, int b)
        {
            makeroot(a);
            access(b);
            fa[a] = 0, ch[b][0] = 0;
        }
        int fdr(int a)
        {
            access(a);
            while(1)
            {
                pushdown(a);
                if (ch[a][0]) a = ch[a][0];
                else {
                    splay(a);
                    return a;
                }
            }
        }
    }
    vi out, in;
    vi work(int n, vector<pi> eg, ll mx_ch = -1) { 
        // mx_ch : max number of adding/replacing  default is (n + 100) * (n + 50) 
        // n : number of vertices. 1-indexed. 
        // eg: vector<pair<int, int> > storing all the edges. 
        // return a vector<int> consists of all indices of vertices on the path. return empty list if failed to find one.  
        out.resize(n + 1), in.resize(n + 1);
        LCT::init(n);
        for (int i = 0; i <= n; i++) in[i] = out[i] = 0;
        if (mx_ch == -1) mx_ch = 1ll * (n + 100) * (n + 50); //default
        vector<vi> from(n + 1), to(n + 1);
        for (auto v : eg)
            from[v.fi].pb(v.se), 
            to[v.se].pb(v.fi);
        unordered_set<int> canin, canout;
        for (int i = 1; i <= n; i++)
            canin.insert(i), 
            canout.insert(i); 
        mt19937 x(chrono::steady_clock::now().time_since_epoch().count());
        int tot = 0;
        while (mx_ch >= 0) {
        //    cout << tot << ' ' << mx_ch << endl;
            vector<pi> eg;
            for (auto v : canout)
                for (auto s : from[v])
                    if (in[s] == 0) {
                        assert(canin.count(s));
                        continue;
                    }
                    else eg.pb(mp(v, s));
            for (auto v : canin)
                for (auto s : to[v])
                    eg.pb(mp(s, v));
            shuffle(eg.begin(), eg.end(), x);
            if (eg.size() == 0) break;
            for (auto v : eg) {
                mx_ch--;
                if (in[v.se] && out[v.fi]) continue;
                if (LCT::fdr(v.fi) == LCT::fdr(v.se)) continue;
                if (in[v.se] || out[v.fi]) 
                    if (x() & 1) continue;
                if (!in[v.se] && !out[v.fi]) 
                    tot++;
                if (in[v.se]) {
                    LCT::cut(in[v.se], v.se);
                    canin.insert(v.se);
                    canout.insert(in[v.se]);
                    out[in[v.se]] = 0;
                    in[v.se] = 0;
                }
                if (out[v.fi]) {
                    LCT::cut(v.fi, out[v.fi]);
                    canin.insert(out[v.fi]);
                    canout.insert(v.fi);
                    in[out[v.fi]] = 0;
                    out[v.fi] = 0;
                }
                LCT::link(v.fi, v.se);
                canin.erase(v.se);
                canout.erase(v.fi);
                in[v.se] = v.fi;
                out[v.fi] = v.se;
            }
            if (tot == n - 1) {
                vi cur;
                for (int i = 1; i <= n; i++) 
                    if (!in[i]) {
                        int pl = i;
                        while (pl) {
                            cur.pb(pl), 
                            pl = out[pl];
                        }
                        break;
                    } 
                return cur;
            }
        }
        //failed to find a path
        return vi();
    }
}