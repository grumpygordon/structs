//example for += in node
struct CALC {
    struct Fen {
#define Val ll
        vector<Val> q;
        int n;
        void init(int n_) {
            n = n_;
            q.resize(n);
            fill(q.begin(), q.end(), 0);
        }

        void inc(int i, Val w) {
            for (; i < n; i = (i | (i + 1)))
                q[i] += w;
        }

        Val get(int i) {
            Val w = 0;
            for (; i >= 0; i = (i & (i + 1)) - 1)
                w += q[i];
            return w;
        }
    } f;

    pair<int, int> seg[maxn];
    int tnow;

    void dfs(int v, int par = -1) {
        seg[v].fr = tnow++;
        for (int i : e[v])
            if (i != par) {
                dfs(i, v);
            }
        seg[v].sc = tnow;
    }

    vector<int> a;

    CALC(vector<int> const &a_) {
        a = a_;
        tnow = 0;
        dfs(0);
        f.init(tnow);
        for (int i = 0; i < n; i++) {
            /todo
            f.inc(seg[i].fr, a[i]);
            f.inc(seg[i].sc, -a[i]);
        }
    }

    void upd(int i, int w) {
        /todo
        f.inc(seg[i].fr, w - a[i]);
        f.inc(seg[i].sc, -(w - a[i]));
        a[i] = w;
    }

    int get(int i) {
        return f.get(seg[i].fr);
    }
};
