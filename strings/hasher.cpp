
// prefix hash - p[k] = x^{k-1} * a[0] + x^{k-2} * a[1] + .. + a[k - 1]

// substring hash - x^{len-1} * a[l] + x^{len-2} * a[l + 1] + .. + a[r]
struct Hasher {

    const int X = 1e6 + 3, md = 1e9 + 993;

    vector<int> p, g;

    Hasher(vector<int> const &a) {
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
        return (h1 * (ll)g[l2] + h2) % md;
    }
};
