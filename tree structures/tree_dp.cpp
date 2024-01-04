namespace reroot {
    namespace detail {
        const auto exclusive = [](const auto& a, const auto& base, const auto& merge_into, const std::nullptr_t&, int vertex) {
            int n = (int)a.size();
            using Aggregate = std::decay_t<decltype(base)>;
            std::vector<Aggregate> b(n, base);
            for (int bit = (int)std::__lg(n); bit >= 0; --bit) {
                for (int i = n - 1; i >= 0; --i) b[i] = b[i >> 1];
                int sz = n - (n & !bit);
                for (int i = 0; i < sz; ++i) {
                    int index = (i >> bit) ^ 1;
                    b[index] = merge_into(b[index], a[i], vertex, i);
                }
            }
            return b;
        };
        const auto exclusive_with_inverse = [](const auto& a, const auto& base, const auto& merge_into, const auto& merge_inv, int vertex) {
            int n = (int)a.size();
            using Aggregate = std::decay_t<decltype(base)>;
            Aggregate all = base;
            for (int i = 0; i < n; ++i) all = merge_into(all, a[i], vertex, i);
            std::vector<Aggregate> b(n, all);
            for (int i = 0; i < n; ++i) b[i] = merge_inv(b[i], a[i], vertex, i);
            return b;
        };
        
        // MergeInto : Aggregate * Value * Vertex(int) * EdgeIndex(int) -> Aggregate
        // Base : Vertex(int) -> Aggregate
        // FinalizeMerge : Aggregate * Vertex(int) * EdgeIndex(int) -> Value
        // Optional: MergeInv : Aggregate * Value * Vertex(int) * EdgeIndex(int) -> Aggregate
        const auto rerooter_helper = [](const auto& g, const auto& base, const auto& merge_into, const auto& finalize_merge,
                                        const auto& exclusive_impl, const auto& merge_inv = nullptr) {
            int n = (int)g.size();
            using Aggregate = std::decay_t<decltype(base(0))>;
            using Value = std::decay_t<decltype(finalize_merge(base(0), 0, 0))>;
            std::vector<Value> root_dp(n), dp(n);
            std::vector<std::vector<Value>> edge_dp(n), redge_dp(n);
            
            std::vector<int> bfs, parent(n);
            bfs.reserve(n);
            bfs.push_back(0);
            for (int i = 0; i < n; ++i) {
                int u = bfs[i];
                for (auto v : g[u]) {
                    if (parent[u] == v) continue;
                    parent[v] = u;
                    bfs.push_back(v);
                }
            }
            
            for (int i = n - 1; i >= 0; --i) {
                int u = bfs[i];
                int p_edge_index = -1;
                Aggregate aggregate = base(u);
                for (int edge_index = 0; edge_index < (int)g[u].size(); ++edge_index) {
                    int v = g[u][edge_index];
                    if (parent[u] == v) {
                        p_edge_index = edge_index;
                        continue;
                    }
                    aggregate = merge_into(aggregate, dp[v], u, edge_index);
                }
                dp[u] = finalize_merge(aggregate, u, p_edge_index);
            }
            
            for (auto u : bfs) {
                dp[parent[u]] = dp[u];
                edge_dp[u].reserve(g[u].size());
                for (auto v : g[u]) edge_dp[u].push_back(dp[v]);
                auto dp_exclusive = exclusive_impl(edge_dp[u], base(u), merge_into, merge_inv, u);
                redge_dp[u].reserve(g[u].size());
                for (int i = 0; i < (int)dp_exclusive.size(); ++i) redge_dp[u].push_back(finalize_merge(dp_exclusive[i], u, i));
                root_dp[u] = finalize_merge(n > 1 ? merge_into(dp_exclusive[0], edge_dp[u][0], u, 0) : base(u), u, -1);
                for (int i = 0; i < (int)g[u].size(); ++i) {
                    dp[g[u][i]] = redge_dp[u][i];
                }
            }
     
            return std::make_tuple(std::move(root_dp), std::move(edge_dp), std::move(redge_dp));
        };
    }  // namespace detail
    const auto rerooter = [](const auto& g, const auto& base, const auto& merge_into, const auto& finalize_merge) {
        return detail::rerooter_helper(g, base, merge_into, finalize_merge, detail::exclusive, nullptr);
    };
    const auto rerooter_inv = [](const auto& g, const auto& base, const auto& merge_into, const auto& finalize_merge, const auto& merge_inv) {
        return detail::rerooter_helper(g, base, merge_into, finalize_merge, detail::exclusive_with_inverse, merge_inv);
    };
}  // namespace reroot
 
int main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int n;
    std::cin >> n;
    
    std::vector<int> color(n);
    for (auto &c : color) std::cin >> c;
 
    std::vector<std::vector<int>> g(n);
    for (int i = 1; i < n; ++i) {
        int u, v;
        std::cin >> u >> v;
        --u, --v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
 
    using Aggregate = int;
    using Value = int;
 
    auto base = [](int vertex) -> Aggregate {
        return 0;
    };
    auto merge_into = [](Aggregate vertex_dp, Value neighbor_dp, int vertex, int edge_index) -> Aggregate {
        return vertex_dp + std::max(neighbor_dp, 0);
    };
    
    auto finalize_merge = [&](Aggregate vertex_dp, int vertex, int edge_index) -> Value {
        return vertex_dp + 2 * color[vertex] - 1;
    };
    
    auto merge_inv = [](Aggregate vertex_dp, Value neighbor_dp, int vertex, int edge_index) -> Aggregate {
        return vertex_dp - std::max(neighbor_dp, 0);
    };
 
    // Two options. Either
    // 1. Use base, merge_into, finalize_merge and run in O(n log n)
    //auto [reroot_result, edge_dp, redge_dp] = reroot::rerooter(g, base, merge_into, finalize_merge);
    // 2. Use base, merge_into, finalize_merge, merge_inv and run in O(n)
    auto [reroot_result, edge_dp, redge_dp] = reroot::rerooter_inv(g, base, merge_into, finalize_merge, merge_inv);
    
    for (auto dp : reroot_result) {
        std::cout << dp << ' ';
    }
    std::cout << '\n';
}