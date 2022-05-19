struct node;

using ptr = node*;

struct node {

    static ptr null;

    int sz = 0, s = 0, w = 0, y = rnd();
    ptr l = null, r = null;

    node() = default;

    node(int s_) : sz(1), s(s_), w(s_) {}
};

ptr node::null = new node();

auto null = node::null;

void upd(ptr t) {
    if (t != null) {
        t->sz = 1 + t->l->sz + t->r->sz;
        t->s = t->w + t->l->s + t->r->s;
    }
}

void split(ptr t, ptr &l, ptr &r, int sz) {
    if (t == null) {
        l = r = null;
        return;
    }
    if (t->l->sz < sz) {
        split(t->r, t->r, r, sz - t->l->sz - 1);
        upd(t);
        l = t;
    } else {
        split(t->l, l, t->l, sz);
        upd(t);
        r = t;
    }
}

void merge(ptr &t, ptr l, ptr r) {
    if (l == null) {
        t = r;
        return;
    }
    if (r == null) {
        t = l;
        return;
    }
    if (l->y < r->y) {
        merge(l->r, l->r, r);
        t = l;
        upd(t);
    } else {
        merge(r->l, l, r->l);
        t = r;
        upd(t);
    }
}

void erase(ptr &t) {
    assert(t != null);
    assert(t->s != 0);
    if (t->l->s != 0)
        erase(t->l);
    else if (t->w == 1)
        merge(t, t->l, t->r);
    else
        erase(t->r);
    upd(t);
}
