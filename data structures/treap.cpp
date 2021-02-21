struct treap {
    int y, sz;
    ll x, s;
    treap *l, *r;
};

typedef treap *ptr;

ptr create(ll x) {
    ptr t = new treap;
    t->l = nullptr;
    t->r = nullptr;
    t->sz = 1;
    t->x = t->s = x;
    t->y = rnd();
    return t;
}

int sz(ptr t) {
    return t == nullptr ? 0 : t->sz;
}

ll sum(ptr t) {
    return t == nullptr ? 0 : t->s;
}

void upd(ptr &t) {
    if (t != nullptr) {
        t->sz = 1 + sz(t->l) + sz(t->r);
        t->s = t->x + sum(t->l) + sum(t->r);
    }
}

void split(ptr t, ptr &l, ptr &r, ll key) {
    if (t == nullptr) {
        l = nullptr;
        r = nullptr;
        return;
    }
    if (t->x <= key)
        split(t->r, t->r, r, key), l = t;
    else
        split(t->l, l, t->l, key), r = t;
    upd(l);
    upd(r);
}

void ksplit(ptr t, ptr &l, ptr &r, int key) {
    if (t == nullptr) {
        l = nullptr;
        r = nullptr;
        return;
    }
    if (sz(t->l) < key)
        ksplit(t->r, t->r, r, key - sz(t->l) - 1), l = t;
    else
        ksplit(t->l, l, t->l, key), r = t;
    upd(l);
    upd(r);
}

void merge(ptr &t, ptr l, ptr r) {
    if (l == nullptr) {
        t = r;
        return;
    }
    if (r == nullptr) {
        t = l;
        return;
    }
    if (l->y <= r->y)
        merge(l->r, l->r, r), t = l;
    else
        merge(r->l, l, r->l), t = r;
    upd(t);
}

void insert(ptr &root, ll w) {
    ptr t = create(w);
    ptr t1, t2;
    split(root, t1, t2, w);
    merge(root, t1, t);
    merge(root, root, t2);
}

void go(ptr t) {
    if (t == nullptr)
        return;
    go(t->l);
    cout << t->x << ' ';
    go(t->r);
}
