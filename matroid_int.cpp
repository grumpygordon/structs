#define edge_t const function<vector<int>(vector<char>&, int)>
#define set_t const function<vector<int>(vector<char>&)>

/// in_ans - w in q, all h not in q such that q - w + h - good1
/// out_ans - w not in q, all h in q such that q - h + w - good2
/// add1 - w not in q such that q + w - good1
/// add2 - w not in q such that q + w - good2

vector<char> matroid_int(int n, edge_t &in_ans, edge_t &nin_ans, set_t &add1, set_t &add2) {
    vector<char> inans(n);
    while (1) {
        vector<int> par(n, -1);
        vector<int> que;
        vector<char> fin(n);
        {
            auto q = add1(inans);
            if (q.empty())
                break;
            for (int i : q) {
                par[i] = i;
                que.push_back(i);
            }
            q = add2(inans);
            if (q.empty())
                break;
            bool found = 0;
            for (int i : q) {
                fin[i] = 1;
                if (par[i] != -1) {
                    found = 1;
                    inans[i] = 1;
                    break;
                }
            }
            if (found)
                continue;
        }
        bool found = 0;
        for (int it = 0; it < que.size() && !found; it++) {
            int v = que[it];
            vector<int> e;
            if (inans[v])
                e = in_ans(inans, v);
            else
                e = nin_ans(inans, v);
            for (int u : e)
                if (par[u] == -1) {
                    par[u] = v;
                    que.push_back(u);
                    if (fin[u]) {
                        found = 1;
                        break;
                    }
                }
        }
        if (!found)
            break;
        int v = que.back();
        while (1) {
            inans[v] ^= 1;
            if (v == par[v])
                break;
            v = par[v];
        }
    }
    return inans;
}
