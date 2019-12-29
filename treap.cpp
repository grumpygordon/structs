struct treap{
    int x, y, mn;
    treap *l, *r;
};

typedef treap * ptr;

ptr create(int x){
    ptr t = new treap;
    t->l = nullptr;
    t->r = nullptr;
    t->x = x;
    t->mn = x;
    t->y = rand();
    return t;
}

int mn(ptr t){
    return t == nullptr ? inf : t->mn;
}

void upd(ptr &t){
    if (t != nullptr)
        t->mn = min(t->x, min(mn(t->l), mn(t->r)));
}

void split(ptr t, ptr &l, ptr &r, int key){
    if (t == nullptr){
        l = nullptr;
        r = nullptr;
        return;
    }
    upd(t);
    if (t->x <= key)
        split(t->r, t->r, r, key), l = t;
    else
        split(t->l, l, t->l, key), r = t;
    upd(l);
    upd(r);
    upd(t);
}

void merge(ptr &t, ptr l, ptr r){
    if (l == nullptr){
        t = r;
        return;
    }
    if (r == nullptr){
        t = l;
        return;
    }
    upd(l);
    upd(r);
    if (l->y <= r->y)
        merge(l->r, l->r, r), t = l;
    else
        merge(r->l, l, r->l), t = r;
    upd(t);
}
