
struct hash_my {
    size_t operator()(int x) const {
        x = ((x >> 16) ^ x) * 0x119de1f3;
        x = ((x >> 16) ^ x) * 0x119de1f3;
        x = (x >> 16) ^ x;
        return x;
    }
 
    size_t operator()(long long x) const {
        x = (x ^ (x >> 30)) * (0xbf58476d1ce4e5b9ll);
        x = (x ^ (x >> 27)) * (0x94d049bb133111ebll);
        x = x ^ (x >> 31);
        return x;
    }
};

// open address


template<typename Key, typename Value, size_t N, typename Hash = hash_my>
struct static_hash_table {
    static_assert(__builtin_popcount(N) == 1, "N must be a power of 2");
    array<pair<Key, Value>, N> data;
    array<char, N> state;
    Hash hasher;
 
    const static char STATE_EMPTY = 0;
    const static char STATE_FILLED = 1;
    const static char STATE_REMOVED = 2;
 
    static_hash_table() {
        state.fill(STATE_EMPTY);
    }
 
    int find_ind(const Key &k) const {
        int ind = hasher(k) & (N - 1);
        while (state[ind] != STATE_EMPTY) {
            if (state[ind] == STATE_FILLED && data[ind].first == k) return ind;
            ind = (ind + 1) & (N - 1);
        }
        return -1;
    }
 
    void emplace(const Key &k, const Value &v) {
        (*this)[k] = v;
    }
 
    Value &operator[](const Key &k) {
        int ind = hasher(k) & (N - 1);
        int rem = -1;
        while (state[ind] != STATE_EMPTY) {
            if (state[ind] == STATE_FILLED && data[ind].first == k) {
                return data[ind].second;
            }
            if (rem == -1 && state[ind] == STATE_REMOVED) rem = ind;
            ind = (ind + 1) & (N - 1);
        }
        if (rem != -1) ind = rem;
        data[ind].first = k;
        data[ind].second = Value();
        state[ind] = STATE_FILLED;
        return data[ind].second;
    }
 
    void erase(const Key &k) {
        int ind = find_ind(k);
        if (ind != -1) {
            state[ind] = STATE_REMOVED;
        }
    }
 
    int count(const Key &k) const {
        return find_ind(k) != -1;
    }
};
 
static_hash_table<long long, int, 1 << 20> e;


// chained

template<typename Key, typename Value, size_t N, typename Hash = hash_my>
struct static_hash_table {
    static_assert(__builtin_popcount(N) == 1, "N must be a power of 2");
    array<vector<pair<Key, Value>>, N> data;
    Hash hasher;
 
    static_hash_table() {}
 
    void emplace(const Key &k, const Value &v) {
        (*this)[k] = v;
    }
 
    Value &operator[](const Key &k) {
        int ind = hasher(k) & (N - 1);
        for (auto &i : data[ind])
            if (i.first == k)
                return i.second;
        data[ind].push_back({k, Value()});
        return data[ind].back().second;
    }
 
    void erase(const Key &k) {
        int ind = hasher(k) & (N - 1);
        for (int i = 0; i < data[ind].size(); i++)
            if (data[ind][i].first == k) {
                swap(data[ind][i], data[ind].back());
                data[ind].pop_back();
                break;
            }
    }
 
    int count(const Key &k) const {
        int ind = hasher(k) & (N - 1);
        for (auto &i : data[ind])
            if (i.first == k)
                return 1;
        return 0;
    }
};
 
static_hash_table<long long, int, 1 << 20> e;


#include <ext/pb_ds/assoc_container.hpp>
using namespace __gnu_pbds;

// open adress
gp_hash_table<long long, int, hash_my> e;
