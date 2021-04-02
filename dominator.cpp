//root required, not all nodes need to be reachable

namespace Dominator {

	const int N = maxn;

	vector<int> bucket[N], gi[N], g[N];
	vector<vector<int>> tree;
	int sdom[N], dom[N], par[N], label[N], dsu[N], rev[N], arr[N];

	int tm;

	int ask(int u, int x) {
		if (u == dsu[u]) return x ? -1 : u;
		int v = ask(dsu[u], x + 1);
		if (v < 0) return u;
		if (sdom[label[dsu[u]]] < sdom[label[u]])
			label[u] = label[dsu[u]];
		dsu[u] = v;
		return x ? v : label[u];
	};
	void un(int u, int v) {
		dsu[v] = u;
	};

	void dfs(int v) {
		arr[v] = tm;
		rev[tm] = v;
		label[tm] = sdom[tm] = dsu[tm] = tm;
		++tm;
		for (int k : g[v]) {
			if (arr[k] == -1) {
				dfs(k);
				par[arr[k]] = arr[v];
			}
			gi[arr[k]].push_back(arr[v]);
		}
	};


	vector<vector<int>> dominator_tree(vector<vector<int>> const &g_, int root) {
		tm = 0;
		int n = g_.size();
		tree.clear();
		tree.resize(n);
		for (int i = 0; i < n; i++)
			g[i] = g_[i];
		vector<int> p(n);
		for (int i = 0; i < n; ++i) {
			p[i] = i;
			arr[i] = -1;
		}
		swap(p[root], p[0]);
		swap(g[0], g[root]);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < g[i].size(); ++j) {
				g[i][j] = p[g[i][j]];
			}
		}

		dfs(0);
		//tm - number of reachable nodes

		for (int i = tm - 1; i >= 0; --i) {
			for (int j : gi[i]) {
				sdom[i] = min(sdom[i], sdom[ask(j, 0)]);
			}
			if (i != 0) bucket[sdom[i]].push_back(i);
			for (int w : bucket[i]) {
				int v = ask(w, 0);
				if (sdom[v] == sdom[w]) dom[w] = sdom[w];
				else dom[w] = v;
			}
			if (i != 0) un(par[i], i);
		}
		for (int i = 1; i < tm; ++i) {
			if (dom[i] != sdom[i])
				dom[i] = dom[dom[i]];
			// ребро вниз
			tree[rev[dom[i]]].push_back(rev[i]);
			// если нужно неориентированное дерево
			// tree[rev[i]].push_back(rev[dom[i]]);
		}

		swap(tree[root], tree[0]);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < tree[i].size(); ++j) {
				tree[i][j] = p[tree[i][j]];
			}
		}

		return tree;
	}
}