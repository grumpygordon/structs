// use carefully
// division works VERY SLOW

const double eps = 1e-12, PI = atan2(0, -1);

namespace fft {
    typedef double dbl;

    struct num {
        dbl x, y;

        num() { x = y = 0; }

        num(dbl x_, dbl y_) : x(x_), y(y_) {}
    };

    inline num operator+(num a, num b) { return num(a.x + b.x, a.y + b.y); }

    inline num operator-(num a, num b) { return num(a.x - b.x, a.y - b.y); }

    inline num operator*(num a, num b) { return num(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }

    inline num conj(num a) { return num(a.x, -a.y); }

    int base = 1;
    vector<num> roots = {{0, 0},
                         {1, 0}};
    vector<int> rev = {0, 1};
    const dbl PI = static_cast<dbl>(acosl(-1.0));

    void ensure_base(int nbase) {
        if (nbase <= base) {
            return;
        }
        rev.resize(1 << nbase);
        for (int i = 0; i < (1 << nbase); i++) {
            rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (nbase - 1));
        }
        roots.resize(1 << nbase);
        while (base < nbase) {
            dbl angle = 2 * PI / (1 << (base + 1));
//      num z(cos(angle), sin(angle));
            for (int i = 1 << (base - 1); i < (1 << base); i++) {
                roots[i << 1] = roots[i];
//        roots[(i << 1) + 1] = roots[i] * z;
                dbl angle_i = angle * (2 * i + 1 - (1 << base));
                roots[(i << 1) + 1] = num(cos(angle_i), sin(angle_i));
            }
            base++;
        }
    }

    void fft(vector<num> &a, int n = -1) {
        if (n == -1) {
            n = (int) a.size();
        }
        assert((n & (n - 1)) == 0);
        int zeros = __builtin_ctz(n);
        ensure_base(zeros);
        int shift = base - zeros;
        for (int i = 0; i < n; i++) {
            if (i < (rev[i] >> shift)) {
                swap(a[i], a[rev[i] >> shift]);
            }
        }
        for (int k = 1; k < n; k <<= 1) {
            for (int i = 0; i < n; i += 2 * k) {
                for (int j = 0; j < k; j++) {
                    num z = a[i + j + k] * roots[j + k];
                    a[i + j + k] = a[i + j] - z;
                    a[i + j] = a[i + j] + z;
                }
            }
        }
    }

    vector<num> fa, fb;

#define Val int
#define Res ll
    vector<Res> square(const vector<Val> &a) {
        if (a.empty()) {
            return {};
        }
        int need = (int) a.size() + (int) a.size() - 1;
        int nbase = 1;
        while ((1 << nbase) < need) nbase++;
        ensure_base(nbase);
        int sz = 1 << nbase;
        if ((sz >> 1) > (int) fa.size()) {
            fa.resize(sz >> 1);
        }
        for (int i = 0; i < (sz >> 1); i++) {
            int x = (2 * i < (int) a.size() ? a[2 * i] : 0);
            int y = (2 * i + 1 < (int) a.size() ? a[2 * i + 1] : 0);
            fa[i] = num(x, y);
        }
        fft(fa, sz >> 1);
        num r(1.0 / (sz >> 1), 0.0);
        for (int i = 0; i <= (sz >> 2); i++) {
            int j = ((sz >> 1) - i) & ((sz >> 1) - 1);
            num fe = (fa[i] + conj(fa[j])) * num(0.5, 0);
            num fo = (fa[i] - conj(fa[j])) * num(0, -0.5);
            num aux = fe * fe + fo * fo * roots[(sz >> 1) + i] * roots[(sz >> 1) + i];
            num tmp = fe * fo;
            fa[i] = r * (conj(aux) + num(0, 2) * conj(tmp));
            fa[j] = r * (aux + num(0, 2) * tmp);
        }
        fft(fa, sz >> 1);
        vector<int64_t> res(need);
        for (int i = 0; i < need; i++) {
            res[i] = llround(i % 2 == 0 ? fa[i >> 1].x : fa[i >> 1].y);
        }
        return res;
    }

    vector<Res> multiply(const vector<Val> &a, const vector<Val> &b) {
        if (a.empty() || b.empty()) {
            return {};
        }
        if (a == b) {
            return square(a);
        }
        int need = (int) a.size() + (int) b.size() - 1;
        int nbase = 1;
        while ((1 << nbase) < need) nbase++;
        ensure_base(nbase);
        int sz = 1 << nbase;
        if (sz > (int) fa.size()) {
            fa.resize(sz);
        }
        for (int i = 0; i < sz; i++) {
            int x = (i < (int) a.size() ? a[i] : 0);
            int y = (i < (int) b.size() ? b[i] : 0);
            fa[i] = num(x, y);
        }
        fft(fa, sz);
        num r(0, -0.25 / (sz >> 1));
        for (int i = 0; i <= (sz >> 1); i++) {
            int j = (sz - i) & (sz - 1);
            num z = (fa[j] * fa[j] - conj(fa[i] * fa[i])) * r;
            fa[j] = (fa[i] * fa[i] - conj(fa[j] * fa[j])) * r;
            fa[i] = z;
        }
        for (int i = 0; i < (sz >> 1); i++) {
            num A0 = (fa[i] + fa[i + (sz >> 1)]) * num(0.5, 0);
            num A1 = (fa[i] - fa[i + (sz >> 1)]) * num(0.5, 0) * roots[(sz >> 1) + i];
            fa[i] = A0 + A1 * num(0, 1);
        }
        fft(fa, sz >> 1);
        vector<int64_t> res(need);
        for (int i = 0; i < need; i++) {
            res[i] = llround(i % 2 == 0 ? fa[i >> 1].x : fa[i >> 1].y);
        }
        return res;
    }
}

// don't increase that, fft will overflow
const Val base = 1e5;
const int basen = 5;

// you can change base to any number,
// but make sure basen is not used

#define pb push_back
struct Big {
    vector<Val> v;
    bool minus = false;

    Big(Res n = 0) : minus(n < 0) {
        n = abs(n);
        while (n != 0) {
            v.pb(n % base);
            n /= base;
        }
        if (v.empty()) {
            v.pb(0);
        }
    }

    void norm() {
        if (v.size() == 0)
            v = {0};
        else
            while (v.size() > 1 && v.back() == 0)
                v.pop_back();
        if (v.size() == 1 && v[0] == 0)
            minus = false;
    }

    void assign(string &s) {
        v.clear();
        reverse(s.begin(), s.end());
        if (s.back() == '-') {
            s.pop_back();
            minus = true;
        }
        while (s.size() % basen != 0)
            s.push_back('0');
        for (int i = 0; i < s.size() / basen; ++i) {
            long long k = 0;
            long long p10 = 1;
            for (int j = 0; j < basen; ++j) {
                k += p10 * (s[i * basen + j] - '0');
                p10 *= 10;
            }
            v.push_back(k);
        }
        norm();
    }

    auto size() const {
        return v.size();
    }

    auto &operator[](int i) const {
        return v[i];
    }

    auto &operator[](int i) {
        return v[i];
    }

    auto &back() {
        return v.back();
    }

    void resize(int k) {
        v.resize(k);
    }

    auto mul10(int k = 1) {
        if (k / basen > 0) {
            v.resize(v.size() + k / basen, 0);
            for (int i = (int) v.size() - k / basen - 1; i >= 0; --i) {
                v[i + k / basen] = v[i];
            }
            for (int i = 0; i < k / basen; ++i) {
                v[i] = 0;
            }
            k %= basen;
        }
        long long p10 = 1;
        for (int i = 0; i < k; ++i) {
            p10 *= 10;
        }
        for (int i = 0; i < v.size(); ++i) {
            v[i] *= p10;
        }
        long long rem = 0;
        for (int i = 0; i < v.size(); ++i) {
            v[i] += rem;
            rem = v[i] / base;
            v[i] %= base;
        }
        if (rem != 0) {
            v.push_back(rem);
            assert(rem < base);
        }
        return *this;
    }

    auto div10(int k = 1) {
        if (k % basen != 0) {
            mul10(basen - k % basen);
            k += basen - k % basen;
        }
        if (k / basen >= v.size()) {
            v = {0};
            return *this;
        }
        for (int i = k / basen; i < v.size(); ++i) {
            v[i - k / basen] = v[i];
        }
        for (int i = (int) v.size() - k / basen; i < v.size(); ++i) {
            v[i] = 0;
        }
        norm();
        return *this;
    }

    void add10(int k = 1) {
        while (k / basen >= v.size())
            v.push_back(0);
        v.push_back(0);
        int p10 = 1;
        for (int i = 0; i < k % basen; ++i) {
            p10 *= 10;
        }
        v[k / basen] += p10;
        int i = k / basen;
        long long rem = 0;
        while (v[i] >= base || rem > 0) {
            rem = v[i] / base;
            v[i] %= base;
            ++i;
        }
        norm();
    }

};

istream &operator>>(istream &in, Big &b) {
    string s;
    cin >> s;
    b.assign(s);
    return in;
}

bool operator==(Big a, Big b) {
    return a.minus == b.minus && a.v == b.v;
}

bool operator==(Big &a, Res k) {
    return (a == Big(k));
}

bool operator!=(Big a, Big b) {
    return !(a == b);
}

bool operator<(Big a, Big b) {
    if (a.minus != b.minus)
        return a.minus;
    bool minus = a.minus;
    if (a.size() != b.size())
        return (a.size() < b.size()) ^ minus;
    for (int i = (int) a.size() - 1; i >= 0; --i)
        if (a[i] != b[i])
            return (a[i] < b[i]) ^ minus;
    return false;
}

bool abs_comp(const Big &a, const Big &b) {
    if (a.size() != b.size())
        return (a.size() < b.size());
    for (int i = (int) a.size() - 1; i >= 0; --i)
        if (a[i] != b[i])
            return (a[i] < b[i]);
    return false;
}

Big operator-(Big a) {
    a.minus = !a.minus;
    return a;
}

Big operator-(Big a, Big b);

Big operator+(Big a, Big b) {
    if (a.minus == b.minus) {
        Big c;
        c.resize(max(a.size(), b.size()) + 3);
        for (int i = 0; i < a.size(); ++i) {
            c[i] += a[i];
        }
        for (int i = 0; i < b.size(); ++i) {
            c[i] += b[i];
        }
        for (int i = 0; i < c.size(); ++i) {
            if (c[i] >= base) {
                c[i] -= base;
                c[i + 1]++;
            }
        }
        c.minus = a.minus;
        c.norm();
        return c;
    } else {
        if (a.minus) {
            return b - (-a);
        } else {
            return a - (-b);
        }
    }
}

string to_string(Big s);

Big operator-(Big a, Big b) {
    if (a.minus == b.minus) {
        bool minus = false;
        if (abs_comp(a, b)) {
            swap(a, b);
            minus = true;
        }
        for (int i = 0; i < b.size() || a[i] < 0; ++i) {
            if (i < b.size())
                a[i] -= b[i];
            if (a[i] < 0) {
                a[i] += base;
                a[i + 1]--;
            }
        }
        a.minus ^= minus;
        a.norm();
        return a;
    } else {
        if (a.minus) {
            return -((-a) + b);
        } else {
            return a + (-b);
        }
    }
}

Big operator*(Big a, Big b) {
    Big c;
    //assert(!a.minus && !b.minus);
    c.minus = (a.minus ^ b.minus);
    auto v = fft::multiply(a.v, b.v);
    c.v.resize(v.size() + 3);
    Res cur = 0;
    for (int i = 0; i < c.v.size(); i++) {
        if (i < v.size())
            cur += v[i];
        c[i] = (cur % base);
        //fft overflows
        assert(c[i] >= 0);
        cur /= base;
    }
    c.norm();
    return c;
    c.resize(a.size() + b.size() + 3);
    c.minus = a.minus ^ b.minus;
    for (int i = 0; i < a.size(); ++i) {
        for (int j = 0; j < b.size(); ++j) {
            c[i + j] += a[i] * b[j];
            if (c[i + j] >= base) {
                c[i + j + 1] += c[i + j] / base;
                c[i + j] %= base;
            }
        }
    }
    for (int i = 0; i < c.size(); ++i) {
        if (c[i] >= base) {
            c[i + 1] += c[i] / base;
            c[i] %= base;
        }
    }
    c.norm();
    return c;
}

pair<Big, Big> div(Big a, Big b) {
    bool minus = a.minus ^ b.minus;
    a.minus = false;
    b.minus = false;
    Big c;
    if (a < b) {
        return {c, a};
    }
    int l = 0, r = a.size() * basen + 10;
    while (r - l > 1) {
        int m = (l + r) / 2;
        b.mul10(m);
        if (a < b) {
            r = m;
        } else {
            l = m;
        }
        b.div10(m);
    }
    int shift = l;
    while (!(a < b) && shift >= 0) {
        b.mul10(shift);
        while (!(a < b)) {
            a = a - b;
            c.add10(shift);
        }
        b.div10(shift);
        --shift;
    }
    c.minus = minus;
    if (c == 0)
        c.minus = false;
    return {c, a};
}

Big operator%(Big a, Big b) {
    return div(a, b).second;
}

string to_string(Big a) {
    string s = a.minus ? "-" : "";
    s += to_string(a.v.back());
    for (int i = (int) a.size() - 2; i >= 0; --i) {
        string s1 = to_string(a[i]);
        assert(s1[0] != '-');
        if (s1.size() % basen != 0) {
            for (int j = 0; j < basen - (s1.size() % basen); ++j) {
                s.push_back('0');
            }
        }
        s += s1;
    }
    return s;
}

Big operator %(Big a, int b) {
    if (b < base) {
        int rem = 0;
        for (int i = (int) a.size() - 1; i >= 0; --i)
            rem = (rem * (ll) base + a[i]) % b;
        return rem;
    }
    return div(a, b).second;
}

Big operator / (Big a, int b) {
    if (b < base) {
        for (int i = (int)a.size() - 1; i >= 0; --i) {
            if (i != 0) {
                a[i - 1] += a[i] % b * base;
            }
            a[i] /= b;
        }
        a.norm();
        return a;
    }
    return div(a, b).first;
}


template<typename T>
Big convolution(vector<T> &ss, int l, int r) {
    if (l == r) {
        return Big(ss[l]);
    }
    int m = (l + r) / 2;
    return convolution(ss, l, m) * convolution(ss, m + 1, r);
}

template<typename T>
Big convolution(vector<T> s) {
    if (s.empty())
        return Big(1);
    return convolution(s, 0, s.size() - 1);
}
