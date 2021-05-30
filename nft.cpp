//you can use both multiply_nft and multiply
//multiply works a little bit faster

using uint = unsigned int;
using ull = unsigned long long;

template <uint MD> struct ModInt {
    using M = ModInt;
    static constexpr uint get_mod() { return MD; }
    const static M G;
    uint v;
    ModInt(ll _v = 0) { set_v(uint(_v % MD + MD)); }
    M& set_v(uint _v) {
        v = (_v < MD) ? _v : _v - MD;
        return *this;
    }
    explicit operator bool() const { return v != 0; }
    M operator-() const { return M() - *this; }
    M operator+(const M& r) const { return M().set_v(v + r.v); }
    M operator-(const M& r) const { return M().set_v(v + MD - r.v); }
    M operator*(const M& r) const { return M().set_v(uint(ull(v) * r.v % MD)); }
    M operator/(const M& r) const { return *this * r.inv(); }
    M& operator+=(const M& r) { return *this = *this + r; }
    M& operator-=(const M& r) { return *this = *this - r; }
    M& operator*=(const M& r) { return *this = *this * r; }
    M& operator/=(const M& r) { return *this = *this / r; }
    bool operator==(const M& r) const { return v == r.v; }
    M pow(ll n) const {
        M x = *this, r = 1;
        while (n) {
            if (n & 1) r *= x;
            x *= x;
            n >>= 1;
        }
        return r;
    }
    M inv() const { return pow(MD - 2); }
    friend ostream& operator<<(ostream& os, const M& r) { return os << r.v; }
};



using Mint = ModInt<998244353>;
template<> const Mint Mint::G = Mint(3);
/*
void findG() {
    ll mod = 786433;
    vector<long long> d;
    long long n = mod - 1;
    if (!(n & 1)) {
        d.push_back(2);
        while (!(n & 1)) {
            n >>= 1;
        }
    }
    long long p = 3;
    while (n > 1) {
        if (n < p * p) break;
        if (n % p == 0) {
            d.push_back(p);
            while (n % p == 0) {
                n /= p;
            }
        }
        p += 2;
    }
    if (n != 1) d.push_back(p);
    n = mod - 1;
    for (auto& item : d) {
        item = n / item;
    }
    Mint g = 2;
    while (true) {
        bool ok = true;
        for (auto k : d) {
            if (g.pow(k) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) {
            break;
        }
        g += 1;
    }
    cerr << g;
}
*/
#define V vector
template <class Mint> void nft(bool type, V<Mint>& a) {
    int n = int(a.size()), s = 0;
    while ((1 << s) < n) s++;
    assert(1 << s == n);

    static V<Mint> ep, iep;
    while (int(ep.size()) <= s) {
        ep.push_back(Mint::G.pow(Mint(-1).v / (1 << ep.size())));
        iep.push_back(ep.back().inv());
    }
    V<Mint> b(n);
    for (int i = 1; i <= s; i++) {
        int w = 1 << (s - i);
        Mint base = type ? iep[i] : ep[i], now = 1;
        for (int y = 0; y < n / 2; y += w) {
            for (int x = 0; x < w; x++) {
                auto l = a[y << 1 | x];
                auto r = now * a[y << 1 | x | w];
                b[y | x] = l + r;
                b[y | x | n >> 1] = l - r;
            }
            now *= base;
        }
        swap(a, b);
    }
}

template <class Mint> V<Mint> multiply_nft(const V<Mint>& a, const V<Mint>& b) {
    int n = int(a.size()), m = int(b.size());
    if (!n || !m) return {};
    if (min(n, m) <= 8) {
        V<Mint> ans(n + m - 1);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) ans[i + j] += a[i] * b[j];
        return ans;
    }
    int lg = 0;
    while ((1 << lg) < n + m - 1) lg++;
    int z = 1 << lg;
    auto a2 = a, b2 = b;
    a2.resize(z);
    b2.resize(z);
    nft(false, a2);
    nft(false, b2);
    for (int i = 0; i < z; i++) a2[i] *= b2[i];
    nft(true, a2);
    a2.resize(n + m - 1);
    Mint iz = Mint(z).inv();
    for (int i = 0; i < n + m - 1; i++) a2[i] *= iz;
    return a2;
}



int bsf(ull x) { return __builtin_ctzll(x); }

// Cooley-Tukey: input -> butterfly -> bit reversing -> output から
// bit reversingを抜いたもの 直接使うな
template <class Mint> void butterfly(bool type, V<Mint>& a) {
    int n = int(a.size()), h = 0;
    while ((1 << h) < n) h++;
    assert(1 << h == n);
    if (n == 1) return;

    static V<Mint> snow, sinow;
    if (snow.empty()) {
        Mint sep = Mint(1), siep = Mint(1);
        uint mod = Mint(-1).v;
        uint di = 4;
        while (mod % di == 0) {
            Mint ep = Mint::G.pow(mod / di);
            Mint iep = ep.inv();
            snow.push_back(siep * ep);
            sinow.push_back(sep * iep);
            sep *= ep;
            siep *= iep;
            di *= 2;
        }
    }

    if (!type) {
        // fft
        for (int ph = 1; ph <= h; ph++) {
            // phase ph: size w -> 2w の FFT, p 並列
            int w = 1 << (ph - 1), p = 1 << (h - ph);
            Mint now = Mint(1);
            for (int s = 0; s < w; s++) {
                int offset = s << (h - ph + 1);
                for (int i = 0; i < p; i++) {
                    auto l = a[i + offset];
                    auto r = a[i + offset + p] * now;
                    a[i + offset] = l + r;
                    a[i + offset + p] = l - r;
                }
                int u = bsf(~uint(s));
                now *= snow[u];
            }
        }
    } else {
        // ifft
        for (int ph = h; ph >= 1; ph--) {
            int w = 1 << (ph - 1), p = 1 << (h - ph);
            Mint inow = Mint(1);
            for (int s = 0; s < w; s++) {
                int offset = s << (h - ph + 1);
                for (int i = 0; i < p; i++) {
                    auto l = a[i + offset];
                    auto r = a[i + offset + p];
                    a[i + offset] = l + r;
                    a[i + offset + p] = (l - r) * inow;
                }
                int u = bsf(~uint(s));
                inow *= sinow[u];
            }
        }
    }
}

template <class Mint> V<Mint> multiply(const V<Mint>& a, const V<Mint>& b) {
    int n = int(a.size()), m = int(b.size());
    if (!n || !m) return {};
    if (min(n, m) < 8) {
        V<Mint> ans(n + m - 1);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) ans[i + j] += a[i] * b[j];
        return ans;
    }
    int lg = 0;
    while ((1 << lg) < n + m - 1) lg++;
    int z = 1 << lg;
    auto a2 = a;
    a2.resize(z);
    butterfly(false, a2);
    if (a == b) {
        for (int i = 0; i < z; i++) a2[i] *= a2[i];
    } else {
        auto b2 = b;
        b2.resize(z);
        butterfly(false, b2);
        for (int i = 0; i < z; i++) a2[i] *= b2[i];
    }
    butterfly(true, a2);
    a2.resize(n + m - 1);
    Mint iz = Mint(z).inv();
    for (int i = 0; i < n + m - 1; i++) a2[i] *= iz;
    return a2;
}
