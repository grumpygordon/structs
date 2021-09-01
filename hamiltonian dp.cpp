// task: given nodes on a line, you pick some in some order and say it's hamiltonian path on them
// task: count length modulo x (you can do anything else regarding length)
// 150 - length of array
// 50 - remainder modulo x of length of hamiltonian path
// 10 - number of nodes in hamiltonian

Mint dp[151][51][11][11][3][3];

void solve() {
    // i , r, y , < , ->
    int n, x, y;
    cin >> n >> x >> y;
    auto md = [&](int w) {
        w %= x;
        if (w < 0)
            w += x;
        return w;
    };
    vector<int> a(n);
    for (int i = 1; i < n; i++) {
        cin >> a[i];
        a[i] += a[i - 1];
    }
    // Для каждого *< мы знаем из какого конца в какой будет идти путь
    // Первый *- | -* это начало, второй это конец
    // В конце домножаем на два чтобы посчитать развернутые пути


    // i - passed, r - remainder mod x
    // passed - number of nodes
    // go - active *<
    // t - active *-, op - passed -* and *-
    Mint ans = 0;
    dp[0][0][0][0][0][0] = 1;
    for (int i = 0; i < n; i++) {
        for (int r = 0; r < x; r++)
            for (int passed = 0; passed <= y; passed++)
                for (int go = 0; go <= passed; go++)
                    for (int t = 0; t < 3; t++)
                    for (int op = 0; op < 3; op++) {
                        Mint value = dp[i][r][passed][go][t][op];
                        if (value != 0) {
                            //cerr << i << ' ' << r << ' ' << passed << ' ' << go << ' ' << t << ' ' << op << ' ' << value << '\n';
                            dp[i + 1][r][passed][go][t][op] += value;
                            if (passed < y) {
                                // -*-
                                dp[i + 1][r][passed + 1][go][t][op] += value * (go * 2 + t);
                                // >*
                                int rv = md(r + 2 * a[i]);
                                if (go > 0) {
                                    dp[i + 1][rv][passed + 1][go - 1][t][op] += value * (go * (go - 1));
                                    dp[i + 1][rv][passed + 1][go - 1][t][op] += value * go * t;
                                }
                                if (t == 2) {
                                    dp[i + 1][rv][passed + 1][go][0][op] += value;
                                }
                                // *<
                                rv = md(r - 2 * a[i]);
                                dp[i + 1][rv][passed + 1][go + 1][t][op] += value;
                                if (op < 2) {
                                    // *-
                                    dp[i + 1][md(r - a[i])][passed + 1][go][t + 1][op + 1] += value;
                                    // -*
                                    rv = md(r + a[i]);
                                    if (go > 0) {
                                        dp[i + 1][rv][passed + 1][go - 1][t + 1][op + 1] += value * go;
                                    }
                                    if (t > 0) {
                                        dp[i + 1][rv][passed + 1][go][t - 1][op + 1] += value * t;
                                    }
                                }
                            }
                        }
                    }
    }
    for (int r = 0; r < 1; r++) {
        ans += dp[n][r][y][0][0][2];
        //cerr << r << ' ' << ans << '\n';
    }
    cout << ans * 2;
}
