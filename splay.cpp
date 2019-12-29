struct splay{
    int x, mn, mx;
    splay *l, *r;
};

typedef splay * ptr;

ptr create(int x){
    ptr ret = new splay;
    ret->l = nullptr;
    ret->r = nullptr;
    ret->x = x;
    ret->mn = x;
    ret->mx = x;
    return ret;
}

int mx(ptr t){
    return t == nullptr ? -inf : t->mx;
}

int mn(ptr t){
    return t == nullptr ? inf : t->mn;
}

ptr rec(ptr t){
    if (t == nullptr)
        return t;
    t->mx = max(t->x, max(mx(t->l), mx(t->r)));
    t->mn = min(t->x, min(mn(t->l), mn(t->r)));
    return t;
}

ptr zig(ptr t, bool type){
    ptr x;
    if (type == 0){
        x = t->l;
        t->l = t->l->r;
        x->r = t;
        x->r = rec(x->r);
        x = rec(x);
    }
    else{
        x = t->r;
        t->r = t->r->l;
        x->l = t;
        x->l = rec(x->l);
        x = rec(x);
    }
    return x;
}

ptr zigzig(ptr t, bool type){
    return zig(zig(t, type), type);
}

ptr zigzag(ptr t, bool type){
    if (type == 0)
        t->l = zig(t->l, 1), t = zig(t, 0);
    else
        t->r = zig(t->r, 0), t = zig(t, 1);
    return t;
}

ptr ksplay(ptr t, int x, bool &evn){
    if (t->x == x){
        evn = 1;
        return t;
    }
    if (t->x > x)
        t->l = ksplay(t->l, x, evn);
    else
        t->r = ksplay(t->r, x, evn);
    evn ^= 1;
    if (evn)
        if (t->x > x)
            if (t->l->x > x)
                return zigzig(t, 0);
            else
                return zigzag(t, 0);
        else
            if (t->r->x > x)
                return zigzag(t, 1);
            else
                return zigzig(t, 1);
    return t;
}

ptr splay(ptr t, int x){
    bool evn = 0;
    t = ksplay(t, x, evn);
    if (t->x != x)
        if (t->x > x)
            return zig(t, 0);
        else
            return zig(t, 1);
    return t;
}

int kexist(ptr t, int x){
    if (t == nullptr)
        return inf;
    if (t->x == x)
        return x;
    if (t->x > x)
        if (t->l == nullptr)
            return t->x;
        else
            return kexist(t->l, x);
    else
        if (t->r == nullptr)
            return t->x;
        else
            return kexist(t->r, x);
}

bool exist(ptr &t, int x){
    int val = kexist(t, x);
    if (val != inf)
        t = splay(t, val);
    return val == x;
}

int kprev(ptr t, int x){
    if (t == nullptr || mn(t) > x)
        return inf;
    if (t->x == x)
        return t->x;
    if (t->x > x)
        return kprev(t->l, x);
    int val = kprev(t->r, x);
    if (val != inf)
        return val;
    return t->x;
}

int prev(ptr &t, int x){
    int val = kprev(t, x);
    if (val != inf)
        t = splay(t, val);
    return val;
}

int knext(ptr t, int x){
    if (t == nullptr || mx(t) < x)
        return inf;
    if (t->x == x)
        return t->x;
    if (t->x < x)
        return knext(t->r, x);
    int val = knext(t->l, x);
    if (val != inf)
        return val;
    return t->x;
}

int next(ptr &t, int x){
    int val = knext(t, x);
    if (val != inf)
        t = splay(t, val);
    return val;
}

int kmaxel(ptr t){
    if (t == nullptr)
        return inf;
    if (t->r == nullptr)
        return t->x;
    return kmaxel(t->r);
}

int maxel(ptr &t){
    int val = kmaxel(t);
    if (val != inf)
        t = splay(t, val);
    return val;
}

int kminel(ptr t){
    if (t == nullptr)
        return inf;
    if (t->l == nullptr)
        return t->x;
    return kminel(t->l);
}

int minel(ptr &t){
    int val = kminel(t);
    if (val != inf)
        t = splay(t, val);
    return val;
}

void split(ptr t, ptr &l, ptr &r, int key){
    key = prev(t, key);
    if (key == inf){
        l = nullptr;
        r = t;
        return;
    }
    t = splay(t, key);
    r = t->r;
    t->r = nullptr;
    l = t;
    rec(l);
    rec(r);
}

ptr merge(ptr l, ptr r){
    int key = maxel(l);
    if (key == inf)
        return r;
    l = splay(l, key);
    l->r = r;
    rec(l);
    return l;
}

ptr ins(ptr t, int x){
    if (exist(t, x))
        return t;
    ptr t1, t2;
    split(t, t1, t2, x);
    t = create(x);
    t->l = t1;
    t->r = t2;
    rec(t);
    return t;
}

ptr rem(ptr t, int x){
    if (!exist(t, x))
        return t;
    t = splay(t, x);
    t = merge(t->l, t->r);
    return t;
}
