//maroonrk, please open your github :(

using uint = uint32_t;
using ull = uint64_t;
template<class t> using vc=vector<t>;
template<class t> using vvc=vc<vc<t>>;

// bit op
int bsr(ull x) { return 63 - __builtin_clzll(x); }
int bsf(ull x) { return __builtin_ctzll(x); }

struct FastSet {
    static constexpr uint B = 64;
    int n, lg;
    vvc<ull> seg;
    FastSet(int _n,ull val=0) : n(_n) {
        do {
            seg.push_back(vc<ull>((_n + B - 1) / B,val));
            _n = (_n + B - 1) / B;
        } while (_n > 1);
        lg = seg.size();
    }
    bool operator[](int i) const {
        return (seg[0][i / B] >> (i % B) & 1) != 0;
    }
    void set(int i) {
        for (int h = 0; h < lg; h++) {
            seg[h][i / B] |= 1ULL << (i % B);
            i /= B;
        }
    }
    void reset(int i) {
        for (int h = 0; h < lg; h++) {
            seg[h][i / B] &= ~(1ULL << (i % B));
            if (seg[h][i / B]) break;
            i /= B;
        }
    }
    // first >= i
    int next(int i) {
        for (int h = 0; h < lg; h++) {
            if (i / B == seg[h].size()) break;
            ull d = seg[h][i / B] >> (i % B);
            if (!d) {
                i = i / B + 1;
                continue;
            }
            // find
            i += bsf(d);
            for (int g = h - 1; g >= 0; g--) {
                i *= B;
                i += bsf(seg[g][i / B]);
            }
            return i;
        }
        return n;
    }
    // first < i
    int prev(int i) {
        i--;
        for (int h = 0; h < lg; h++) {
            if (i == -1) break;
            ull d = seg[h][i / B] << (63 - i % 64);
            if (!d) {
                i = i / B - 1;
                continue;
            }
            // find
            i += bsr(d) - (B - 1);
            for (int g = h - 1; g >= 0; g--) {
                i *= B;
                i += bsr(seg[g][i / B]);
            }
            return i;
        }
        return -1;
    }
};