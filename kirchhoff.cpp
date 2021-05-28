
using matrix = vector<vector<Mint>>;

Mint det(matrix a) {
    /*for (auto i : a) {
        for (auto j : i)
            cout << (int)j << ' ';
        cout << '\n';
    }*/
    //exit(0);
    int n = a.size() - 1;
    Mint det = 1;
    for (int i=0; i<n; ++i) {
        int k = i;
        for (int j=i+1; j<n; ++j)
            if (a[j][i] != 0)
                k = j;
        if (a[k][i] == 0) {
            det = 0;
            break;
        }
        swap (a[i], a[k]);
        if (i != k)
            det = -det;
        det *= a[i][i];
        for (int j=i+1; j<n; ++j)
            a[i][j] /= a[i][i];
        for (int j=0; j<n; ++j)
            if (j != i && a[j][i] != 0)
                for (int k=i+1; k<n; ++k)
                    a[j][k] -= a[i][k] * a[j][i];
    }

    return det;
}

Mint mst(int n, vector<pair<int, int>> e) {
    matrix q(n, vector<Mint>(n));
    for (auto t : e) {
        int v = t.first, u = t.second;
        q[v][v]++;
        q[u][u]++;
        q[v][u] = q[u][v] = -1;
    }
    return det(q);
}