
//f(a, m) - m-th element of a, m is 1-based
//BM(a) - smallest recurrence
//if q = BM(a), then
//sum(q_i * a_i) = 0 true for any shift of a

//template<typename T>
//use T=Mint
vector<T> BM(vector<T> a) {
    vector<T> p = {1};
    vector<T> q = {1};
    int l = 0;
    for (int r = 1; r <= (int) a.size(); r++) {
        T delta = 0;
        for (int j = 0; j <= l; j++) {
            delta += a[r - 1 - j] * p[j];
        }
        q.insert(q.begin(), 0);
        if (delta != 0) {
            vector<T> t = p;
            if (q.size() > t.size()) {
                t.resize(q.size());
            }
            for (int i = 0; i < (int) q.size(); i++) {
                t[i] -= delta * q[i];
            }
            if (2 * l <= r - 1) {
                q = p;
                T od = 1 / delta;
                for (T& x : q) {
                    x *= od;
                }
                l = r - l;
            }
            swap(p, t);
        }
    }
    assert((int) p.size() == l + 1);
//  assert(l * 2 + 30 < (int) a.size());
    reverse(p.begin(), p.end());
    return p;
}
template<typename T>
vector<T> mul(vector<T> a, vector<T> b) {
    vector<T> c(a.size() + b.size() - 1);
    for (int i = 0; i < (int)a.size(); i++) {
        for (int j = 0; j < (int)b.size(); j++) {
            c[i + j] = (c[i + j] + a[i] * b[j]);
        }
    }
    return c;
}
template<typename T>
vector<T> mem(vector<T> a, vector<T> b) {
    if (a.size() < b.size()) a.resize(b.size() - 1);

    T o = 1 / T(b.back());
    for (int i = (int)a.size() - 1; i >= (int)b.size() - 1; i--) {
        if (a[i] == 0) continue;
        T coef = o * (-a[i]);
        for (int j = 0; j < (int)b.size(); j++) {
            a[i - (int)b.size() + 1 + j] = a[i - (int)b.size() + 1 + j] + coef * b[j];
        }
    }
    while (a.size() >= b.size()) {
        assert(a.back() == 0);
        a.pop_back();
    }
    return a;
}
template<typename T>
vector<T> bin(int n, vector<T> p) {
    vector<T> res(1, 1);
    vector<T> a(2); a[1] = 1;
    while (n) {
        if (n & 1) res = mem(mul(res, a), p);
        a = mem(mul(a, a), p);
        n >>= 1;
    }
    return res;
}

//m is 1-based
template<typename T>
T f(vector<T> t, int m) {
    vector<T> v = BM(t);
    vector<T> o = bin(m - 1, v);
    T res = 0;
    for (int i = 0; i < (int)o.size(); i++) res = res + o[i] * t[i];
    return res;
}