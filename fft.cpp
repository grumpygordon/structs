
// isaf27 orz

const double eps = 1e-12, PI = atan2(0, -1);

namespace fft {
    typedef double dbl;

    struct num {
        dbl x, y;

        num() { x = y = 0; }

        num(dbl x, dbl y) : x(x), y(y) {}
    };

    inline num operator+(num a, num b) { return num(a.x + b.x, a.y + b.y); }

    inline num operator-(num a, num b) { return num(a.x - b.x, a.y - b.y); }

    inline num operator*(num a, num b) { return num(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }

    inline num conj(num a) { return num(a.x, -a.y); }

    int base = 1;
    vector<num> roots = {{0, 0},
                         {1, 0}};
    vector<int> rev = {0, 1};

    const dbl PI = acosl(-1.0);

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
            n = a.size();
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
/*    for (int k = 1; k < n; k <<= 1) {
      for (int i = 0; i < n; i += 2 * k) {
        for (int j = 0; j < k; j++) {
          num z = a[i + j + k] * roots[j + k];
          a[i + j + k] = a[i + j] - z;
          a[i + j] = a[i + j] + z;
        }
      }
    }*/
        for (int len = 1; len < n; len <<= 1) {
            for (int i = 0; i < n; i += 2 * len) {
                for (int j = i, k = i + len; j < i + len; j++, k++) {
                    num z = a[k] * roots[k - i];
                    a[k] = a[j] - z;
                    a[j] = a[j] + z;
                }
            }
        }
    }

    vector<num> fa, fb;
#define Res long long
#define Val int
    vector<Res> multiply(vector<Val> &a, vector<Val> &b) {
        int need = a.size() + b.size() - 1;
        int nbase = 0;
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
        num r(0, -0.25 / sz);
        for (int i = 0; i <= (sz >> 1); i++) {
            int j = (sz - i) & (sz - 1);
            num z = (fa[j] * fa[j] - conj(fa[i] * fa[i])) * r;
            if (i != j) {
                fa[j] = (fa[i] * fa[i] - conj(fa[j] * fa[j])) * r;
            }
            fa[i] = z;
        }
        fft(fa, sz);
        vector<Res> res(need);
        for (int i = 0; i < need; i++) {
            res[i] = fa[i].x + 0.5;
        }
        return res;
    }

// works for max value (max(a)*max(b)*n) up to 1e14 (multiply_mod with n up to 1e5)
// using dbl = long double;  // works for max value (max(a)*max(b)*n) up to 1e17
// dbl - 8 byte
// ldbl - 10 byte
}
