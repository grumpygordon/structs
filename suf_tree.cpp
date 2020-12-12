// $ is added at the end of string
// number of leafs is |s| + 1: equals to the number of suffixes
// number of nodes is 2 * |s| + 1
// e - edges in form e[v] = vector({u, {l, r}}): [l; r] - inclusive
// sufid - node id of each suffix from pos 0 to pos |s| (including $)
// len(e) ~ 2 * |s| + 1
// len(sufid) = |s| + 1
// watch out for MLE!
// all arrays should be twice big!!!!

namespace Suftree {
#define forn(i, n) for (int i = 0; i < (int)(n); i++)

    const int N = maxn, VN = 2 * N;
    vector<vector<pair<int, pair<int, int>>>> e;
    vector<int> sufid;

    char s[N + 1];
    vector<map<char, int>> t;
    int l[VN], r[VN], p[VN]; // ребро p[v] -> v это отрезок [l[v], r[v]) исходной строки

    void gfs(int v, int h) {
        if (e[v].empty()) {
            sufid[h - 1] = v;
        }
        for (auto [i, seg] : e[v])
            gfs(i, h + seg.sc - seg.fr + 1);
    }

    int cc, n, suf[VN], vn = 2, v = 1, pos; // идём по ребру из p[v] в v, сейчас стоим в pos

    // returns number of nodes
    int build(string s_) {
        s_.push_back('$');
        t.resize(VN);
        for (int i = 0; i < (int)s_.size(); i++)
            s[i] = s_[i];
        s[s_.size()] = 0;
        forn(i, 127) t[0][i] = 1; // 0 = фиктивная, 1 = корень
        l[1] = -1;
        for (n = 0; s[n]; n++) {
            char c = s[n];
            auto new_leaf = [&](int v) {
                assert(vn < VN);
                p[vn] = v, l[vn] = n, r[vn] = N, t[v][c] = vn++;
            };
            go:;
            if (r[v] <= pos) {
                if (!t[v].count(c)) {
                    new_leaf(v), v = suf[v], pos = r[v];
                    goto go;
                }
                v = t[v][c], pos = l[v] + 1;
            } else if (c == s[pos]) {
                pos++;
            } else {
                int x = vn++;
                l[x] = l[v], r[x] = pos, l[v] = pos;
                p[x] = p[v], p[v] = x;
                t[p[x]][s[l[x]]] = x, t[x][s[pos]] = v;
                new_leaf(x);
                v = suf[p[x]], pos = l[x];
                while (pos < r[x])
                    v = t[v][s[pos]], pos += r[v] - l[v];
                suf[x] = (pos == r[x] ? v : vn);
                pos = r[v] - (pos - r[x]);
                goto go;
            }
        }
        for (int i = 2; i < vn; i++)
            r[i] = min((int)s_.size(), r[i]);

        e.resize(vn - 1, {});
        for (int i = 2; i < vn; i++)
            e[p[i] - 1].push_back({i - 1, {l[i], r[i] - 1}});

        sufid.resize(s_.size());
        gfs(0, 0);
        reverse(sufid.begin(), sufid.end());

        // clearing t for memory reasons
        vector<map<char, int>> sos;
        swap(sos, t);
        return vn - 1;
    }
}
