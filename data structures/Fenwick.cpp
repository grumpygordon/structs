struct Fen {
#define Val int
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

    // return min i: sum(0, i) >= x
    int lower_bound(Val x) {
        int k = 1;
        while (2 * k <= n) k *= 2;

        int res = 0;
        while (k > 0) {
            if (q[res + k - 1] < x) {
                x -= q[res + k - 1];
                res += k;
            }
            k /= 2;
        } 
        return res;
    }
};