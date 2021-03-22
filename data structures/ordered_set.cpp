#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template<typename T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
// find_by_order, order_of_key


// usage:

// ordered_set<T> q;

// q.insert(z)
// each element is inserted only if not present


// q.order_of_key(z)
// index(0-based) of lower_bound(z)

// q.find_by_order(z)
// iterator of z-th element