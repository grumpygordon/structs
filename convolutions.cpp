// careful with overflow, max value is max(res)*len(res), because of division in the end

void FST(vector<int>& a, bool inv) {
    for (int n = a.size(), step = 1; step < n; step *= 2) {
        for (int i = 0; i < n; i += 2 * step) for (int j = i; j < i + step; j++) {
            int &u = a[j], &v = a[j + step]; tie(u, v) =
                                                     inv ? make_pair(v - u, u) : make_pair(v, u + v); // AND
            inv ? make_pair(v, u - v) : make_pair(u + v, u); // OR
            make_pair(u + v, u - v);                   // XOR
        }
    }
    // precalc inverse and replace with multiplication?
    if (inv) for (auto &x : a) x /= a.size(); // XOR only
}
vector<int> conv(vector<int> a, vector<int> b) {
    FST(a, 0); FST(b, 0);
    for (int i = 0; i < a.size(); i++) a[i] *= b[i];
    FST(a, 1); return a;
}


// c[x] = sum_{y in x} : (a[y] * b[x ^ y])
vector<int> subset_conv(vector<int> a, vector<int> b) {
    int n = a.size();
    int N = 0;
    while (n > 1) {
        N++;
        n /= 2;
    }
    n = a.size();
    // Make fhat[][] = {0} and ghat[][] = {0}
    array<array<int, n>, N + 1> fhat, ghat;



    for (auto &i : fhat)
        i.fill(0);
    for (auto &i : ghat)
        i.fill(0);
    for(int mask = 0; mask < (1 << N); mask++) {
        fhat[__builtin_popcount(mask)][mask] = f[mask];
        ghat[__builtin_popcount(mask)][mask] = g[mask];
    }

    // Apply zeta transform on fhat[][] and ghat[][]
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            for(int mask = 0; mask < (1 << N); mask++) {
                if((mask & (1 << j)) != 0) {
                    fhat[i][mask] += fhat[i][mask ^ (1 << j)];
                    ghat[i][mask] += ghat[i][mask ^ (1 << j)];
                }
            }
        }
    }

    array<array<int, n>, N> h;
    for (auto &i : h)
        i.fill(0);

    // Do the convolution and store into h[][] = {0}
    for(int mask = 0; mask < (1 << N); mask++) {
        for(int i = 0; i < N; i++) {
            for(int j = 0; j <= i; j++) {
                h[i][mask] += fhat[j][mask] * ghat[i - j][mask];
            }
        }
    }

    // Apply inverse SOS dp on h[][]
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            for(int mask = 0; mask < (1 << N); mask++) {
                if((mask & (1 << j)) != 0) {
                    h[i][mask] -= h[i][mask ^ (1 << j)];
                }
            }
        }
    }
    
    vector<int> fog(n);

    for(int mask = 0; mask < (1 << N); mask++)  fog[mask] = h[__builtin_popcount(mask)][mask];
    return fog;
}
