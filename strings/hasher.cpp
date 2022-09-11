// prefix hash - p[k] = x^{k-1} * a[0] + x^{k-2} * a[1] + .. + a[k - 1]
// substring hash - x^{len-1} * a[l] + x^{len-2} * a[l + 1] + .. + a[r]

// make sure X > max(a_i)
struct Hasher {

    const int X = 1e6 + 3, md = 1e9 + 993;

    vector<int> p, g;

    Hasher() {}

    Hasher(int X_, int md_) : X(X_), md(md_) {}

    template<typename T>
    void init(T const &a) {
        p.resize(a.size() + 1);
        g.resize(a.size() + 1);
        g[0] = 1;
        for (int i = 1; i < g.size(); i++)
            g[i] = (g[i - 1] * (ll)X) % md;
        int h = 0;
        for (int i = 0; i < a.size(); i++) {
            h = (h * (ll)X + a[i]) % md;
            p[i + 1] = h;
        }
    }

    int get(int l, int r) const {
        if (l > r)
            return 0;
        int x = (p[r + 1] - (p[l] * (ll)g[r - l + 1])) % md;
        if (x < 0)
            x += md;
        return x;
    }

    // h1 * x^{len2} + h2
    int concat(int h1, int h2, int len2) {
        return (h1 * (ll)g[len2] + h2) % md;
    }
};

struct MultiHasher {
    vector<Hasher> q;

    MultiHasher(vector<int> X, vector<int> md) {
        assert(X.size() == md.size());
        for (int i = 0; i < X.size(); i++)
            q.emplace_back(X[i], md[i]);
    }

    template<typename T>
    void init(T const &a) {
        for (auto &i : q)
            i.init(a);
    }

    vector<int> get(int l, int r) const {
        vector<int> res(q.size());
        for (int i = 0; i < q.size(); i++)
            res[i] = q[i].get(l, r);
        return res;
    }

    vector<int> concat(vector<int> const &h1, vector<int> const &h2, int len2) {
        vector<int> res(q.size());
        for (int i = 0; i < q.size(); i++)
            res[i] = q[i].concat(h1[i], h2[i], len2);
        return res;
    }
};

namespace MultiHasherFactory {
    vector<int> X;
    vector<int> md;

    const int lowest_md = 1e9 + 1000;
    const int default_X = 1e6 + 3;

    void fill(int sz) {
        int next = lowest_md;
        for (int it = 0; it < sz; it++) {
            while (1) {
                bool prime = 1;
                for (int i = 2; i * i <= next; i++)
                    if (next % i == 0) {
                        prime = 0;
                        break;
                    }
                if (prime)
                    break;
                next++;
            }
            md.emplace_back(next);
            X.emplace_back(default_X);
            next++;
        }
    }

    MultiHasher getMultiHasher(int sz) {
        assert(X.size() == md.size());
        if (X.empty())
            fill(sz);
        return MultiHasher(X, md);
    }
}
