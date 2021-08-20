//working polynomials: https://github.com/e-maxx-eng/e-maxx-eng-aux/blob/master/src/polynomial.cpp
//has sqrt: https://github.com/koosaga/olympiad/blob/master/Library/codes/math/algebra.cpp
//some random izban's code

#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

namespace mod {
    typedef long long ll;
    const int MOD = 490019;

    void add(int &a, ll b) {
        a = (a + b) % MOD;
    }

    int mul(int a, int b) {
        return 1LL * a * b % MOD;
    }

    int bin(int a, ll n) {
        int res = 1;
        while (n) {
            if (n & 1) res = 1LL * res * a % MOD;
            a = 1LL * a * a % MOD;
            n >>= 1;
        }
        return res;
    }

    int inv(int x) {
        return bin(x, MOD - 2);
    }
}
using namespace mod;


namespace fft {
    struct num {
        double x, y;
        num() {}
        num(double xx, double yy): x(xx), y(yy) {}
        num(double alp): x(cos(alp)), y(sin(alp)) {}
    };
    inline num operator + (num a, num b) { return num(a.x + b.x, a.y + b.y); }
    inline num operator - (num a, num b) { return num(a.x - b.x, a.y - b.y); }
    inline num operator * (num a, num b) { return num(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
    inline num conj(num a) { return num(a.x, -a.y); }

    const double PI = acos(-1);

    std::vector<num> root;
    std::vector<int> rev;
    std::vector<num> a, b;
    int N;

    void prepRoots() {
        if ((int)root.size() >= N) return;
        int old = root.size();
        root.resize(N);
        if (old == 0) {
            if (root.size() < 2) root.resize(2);
            root[1] = num(1, 0);
        }
        for (int k = 1; (1 << k) < N; k++) {
            if ((1 << k) < old) continue;
            num x(2 * PI / (1LL << (k + 1)));
            for (int i = (1LL << (k - 1)); i < (1LL << (k)); i++) {
                root[2 * i] = root[i];
                root[2 * i + 1] = root[i] * x;
            }
        }
        rev.resize(N);
        for (int i = 1; i < N; i++) rev[i] = (rev[i >> 1] >> 1) | ((i & 1) * (N / 2));
    }

    template<typename T>
    void prepAB(std::vector<T> &A, std::vector<T> &B) {
        N = 1;
        while (N < (int)(A.size() + B.size())) N <<= 1;
        while ((int)A.size() < N) A.push_back(0);
        while ((int)B.size() < N) B.push_back(0);
        prepRoots();
    }

    void fft(std::vector<num> &f) {
        for (int i = 0; i < N; i++) {
            int j = rev[i] >> __builtin_ctz(root.size() / N);
            if (i < j) std::swap(f[i], f[j]);
        }
        for (int k = 1; k < N; k <<= 1) {
            for (int i = 0; i < N; i += 2 * k) {
                for (int j = 0; j < k; j++) {
                    num z = f[i + j + k] * root[j + k];
                    f[i + j + k] = f[i + j] - z;
                    f[i + j] = f[i + j] + z;
                }
            }
        }
    }

    template<typename T>
    std::vector<T> mul(std::vector<T> A, std::vector<T> B) {
        int rsz = A.size() + B.size() - 1;
        if (1LL * A.size() * B.size() <= 3e5) {
            vector<T> C(rsz);
            for (int s = 0; s < rsz; s++) {
                ll cur = 0;
                const int mx = min(s, (int)A.size() - 1);
                const int mn = max(0, s - ((int)B.size() - 1));
                for (int i = mn; i <= mx; i++) {
                    cur += 1LL * A[i] * B[s - i];
                }
                C[s] = cur % MOD;
            }
            return C;
        }
        /*prepAB(A, B);

        if ((int)a.size() < N) a.resize(N);
        for (int i = 0; i < N; i++) a[i] = num(A[i], B[i]);
        fft(a);
        for (int i = 0; i <= N / 2; i++) {
            int j = (N - i) & (N - 1);
            num x = (a[j] * a[j] - conj(a[i] * a[i])) * num(0, -0.25 / N);
            num y = (a[i] * a[i] - conj(a[j] * a[j])) * num(0, -0.25 / N);
            a[i] = x;
            if (j != i) a[j] = y;
        }
        fft(a);
        std::vector<T> C(N);
        for (int i = 0; i < N; i++) C[i] = (T)((ll)round(a[i].x) % MOD);
        C.resize(rsz);
        return C;*/
        prepAB(A, B);

        if ((int)a.size() < N) a.resize(N);
        for (int i = 0; i < N; i++) {
            int x = A[i] % MOD;
            a[i] = num(x & ((1 << 15) - 1), x >> 15);
        }
        fft(a);
        if (A == B) {
            b = a;
        } else {
            if ((int)b.size() < N) b.resize(N);
            for (int i = 0; i < N; i++) {
                int x = B[i] % MOD;
                b[i] = num(x & ((1 << 15) - 1), x >> 15);
            }
            fft(b);
        }
        for (int i = 0; i <= N / 2; i++) {
            int j = (N - i) & (N - 1);
            num a1 = (a[i] + conj(a[j])) * num(0.5, 0);
            num a2 = (a[i] - conj(a[j])) * num(0, -0.5);
            num b1 = (b[i] + conj(b[j])) * num(0.5 / N, 0);
            num b2 = (b[i] - conj(b[j])) * num(0, -0.5 / N);
            if (i != j) {
                num aa1 = (a[j] + conj(a[i])) * num(0.5, 0);
                num aa2 = (a[j] - conj(a[i])) * num(0, -0.5);
                num bb1 = (b[j] + conj(b[i])) * num(0.5 / N, 0);
                num bb2 = (b[j] - conj(b[i])) * num(0, -0.5 / N);
                a[i] = aa1 * bb1 + aa2 * bb2 * num(0, 1);
                b[i] = aa1 * bb2 + aa2 * bb1;
            }
            a[j] = a1 * b1 + a2 * b2 * num(0, 1);
            b[j] = a1 * b2 + a2 * b1;
        }
        fft(a);
        fft(b);
        std::vector<T> C(N);
        for (int i = 0; i < N; i++) {
            long long aa = a[i].x + 0.5;
            long long bb = b[i].x + 0.5;
            long long cc = a[i].y + 0.5;
            C[i] = (aa + bb % MOD * (1LL << 15) + cc % MOD * (1LL << 30)) % MOD;
        }
        C.resize(rsz);
        return C;
    }
}

struct poly : vector<int> {
    using vector<int>::vector;
    poly(const vector<int> &a) : vector<int>(a) {}
    poly(int x) : vector<int>(1, x) {}

    friend ostream& operator<< (ostream& out, const poly& p) {
        out << p.to_string();
        return out;
    }

    int& operator[](int id) {
        if (id >= (int)size()) resize(id + 1);
        assert(id >= 0 && id < (int)size());
        return vector<int>::operator[](id);
    }

    int operator[](int id) const {
        assert(id >= 0);
        if (id < (int)size()) return vector<int>::operator[](id);
        return 0;
    }

    poly operator+ (const poly &b) {
        const poly &a = *this;
        poly c(max(a.size(), b.size()));
        for (int i = 0; i < (int)c.size(); i++) {
            c[i] = (a[i] + b[i]) % MOD;
        }
        return c;
    }

    poly operator+= (const poly &p) {
        return *this = *this + p;
    }

    poly operator- (const poly &b) {
        const poly &a = *this;
        poly c(max(a.size(), b.size()));
        for (int i = 0; i < (int)c.size(); i++) {
            c[i] = (a[i] - b[i] + MOD) % MOD;
        }
        return c;
    }

    poly operator-= (const poly &p) {
        return *this = *this - p;
    }

    poly operator* (int d) const {
        poly res = *this;
        for (int i = 0; i < (int)size(); i++) {
            res[i] = 1LL * res[i] * d % MOD;
        }
        return res;
    }

    poly operator*= (int d) {
        return *this = *this * d;
    }

    poly operator/ (int d) const {
        poly res = *this;
        d = ::inv(d);
        for (int i = 0; i < (int)size(); i++) {
            res[i] = 1LL * res[i] * d % MOD;
        }
        return res;
    }

    poly operator/= (int d) {
        return *this = *this / d;
    }

    poly operator* (const poly &p) const {
        return poly(fft::mul(*this, p));
    }

    poly operator*= (const poly &p) {
        return *this = *this * p;
    }

    poly inv() const {
        assert((*this)[0] != 0);
        poly r(1, ::inv((*this)[0]));
        for (int n = 1; n < (int)size(); n <<= 1) {
            poly ca = cut(2 * n);
            r = (r * 2 - r * r * ca).cut(2 * n);
        }
        r.resize(size());
        return r;
    }

    void norm() {
        while (size() > 1 && back() == 0) {
            pop_back();
        }
    }

    poly rev() const {
        return poly(rbegin(), rend());
    }

    poly cut(int n) const {
        poly res = *this;
        res.resize(n);
        return res;
    }

    pair<poly, poly> div(poly b) const {
        auto a = *this;
        a.norm(); b.norm();
        if (a.size() < b.size()) return make_pair(poly({0}), a);

        int x = b.back();
        assert(x != 0);
        b /= x;

        auto rb = b.rev().cut(a.size() - b.size() + 1);
        auto q = (rb.inv() * a.rev()).cut(a.size() - b.size() + 1).rev();

        poly r = a - b * q;
        r.resize(b.size() - 1);
        q /= x;
        r.norm();
        return {q, r};
    }

    poly operator/ (const poly &p) const {
        return div(p).first;
    }

    poly operator/= (const poly &p) {
        return *this = *this / p;
    }

    poly operator% (const poly &p) const {
        return div(p).second;
    }

    poly operator%= (const poly &p) {
        return *this = *this % p;
    }

    string to_string() const {
        string res = "";
        for (int i = 0; i < (int)size(); i++) {
            res += ::to_string((*this)[i]);
            if (i + 1 < (int)size()) res += " ";
        }
        return res;
    }

    bool operator==(const poly &rgh) const {
        return (vector<int>)*this == (vector<int>)rgh;
    }

    int eval(int x) const {
        ll res = 0;
        for (int i = (int)size() - 1; i >= 0; i--) {
            res = (res * x + (*this)[i]) % MOD;
        }
        return res;
    }
};

int solve(int n, int m, int c, vector<int> a, vector<int> b) {
    if (c == 0) return 0;
    vector<int> x(m);
    for (int j = 0; j < m; j++) x[j] = bin(c, j);

    vector<poly> t(4 * m);
    function<poly(int, int, int)> prod = [&](int v, int l, int r) {
        poly res;
        if (l == r) {
            res = poly({(MOD - x[l]) % MOD, 1});
        } else {
            int m = (l + r) >> 1;
            res = prod(v * 2, l, m) * prod(v * 2 + 1, m + 1, r);
        }
        return t[v] = res;
    };
    prod(1, 0, m - 1);

    vector<int> ans(m);

    function<void(poly, int, int, int)> eval = [&](poly cur, int v, int l, int r) {
        cur %= t[v];
        if (r - l <= 1000) {
            for (int i = l; i <= r; i++) {
                ans[i] = cur.eval(x[i]);
            }
        } else {
            int m = (l + r) >> 1;
            eval(cur, v * 2, l, m);
            eval(cur, v * 2 + 1, m + 1, r);
        }
    };
    eval(poly(a), 1, 0, m - 1);

    int res = 0;
    for (int j = 0; j < m; j++) {
        res = (res + 1LL * b[j] * ans[j]) % MOD;
    }

    return res;
}

void test() {
    const int N = 1.5e5;
    int n = N, m = N;
    int c = 2;
    vector<int> a(n), b(m);
    for (int i = 0; i < n; i++) a[i] = rand() % 1001;
    for (int i = 0; i < m; i++) b[i] = rand() % 1001;
    cout << solve(n, m, c, a, b) << endl;
    cout << clock() / (double)CLOCKS_PER_SEC << endl;
    exit(0);
}

int main() {
#ifdef HOME
    freopen("in", "r", stdin);
#endif

    //test();

    int n, m, c;
    while (scanf("%d%d%d", &n, &m, &c) == 3) {
        vector<int> a(n), b(m);
        for (int i = 0; i < n; i++) scanf("%d", &a[i]);
        for (int i = 0; i < m; i++) scanf("%d", &b[i]);

        cout << solve(n, m, c, a, b) << endl;
    }
    return 0;
}