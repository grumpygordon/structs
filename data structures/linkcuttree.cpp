struct splay{
    int x, sz, pp;
    bool rev;
    splay *l, *r, *p;
};

typedef splay * ptr;

ptr create(int x){
    ptr ret = new splay;
    ret->l = nullptr;
    ret->r = nullptr;
    ret->x = x;
    ret->rev = 0;
    ret->sz = 1;
    ret->p = nullptr;
    ret->pp = -1;
    return ret;
}

int sz(ptr t){
    return t == nullptr ? 0 : t->sz;
}

bool notnull(ptr t){
    return t != nullptr;
}

int getx(ptr t){
    return t == nullptr ? -inf : t->x;
}

void push(ptr &t){
    if (t == nullptr)
        return;
    if (t->rev){
        swap(t->l, t->r);
        if (t->l != nullptr)
            t->l->rev ^= 1;
        if (t->r != nullptr)
            t->r->rev ^= 1;
        t->rev = 0;
    }
}

int getson(ptr t, int x){
    return getx(t->r) == x;
}

ptr rec(ptr t){
    if (t == nullptr)
        return t;
    t->sz = 1 + sz(t->l) + sz(t->r);
    if (t->l != nullptr)
        t->l->p = t;
    if (t->r != nullptr)
        t->r->p = t;
    return t;
}

ptr zig(ptr t, bool type){
    ptr x;
    if (type == 0){
        x = t->l;
        t->l = t->l->r;
        x->r = t;
        x->r = rec(x->r);
    }
    else{
        x = t->r;
        t->r = t->r->l;
        x->l = t;
        x->l = rec(x->l);
    }
    x->p = t->p;
    if (t->p != nullptr)
        if (getson(t->p, t->x))
            t->p->r = x;
        else
            t->p->l = x;
    x = rec(x);
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

ptr fgetpath[maxn];

int szgetpath;

void getpath(ptr t){
    if (notnull(t->p))
        getpath(t->p);
    fgetpath[szgetpath] = t;
    szgetpath++;
}

ptr kdsplay(ptr t, int start, bool evn, int &ppval){
    if (t->p == nullptr)
        ppval = t->pp, t->pp = -1;
    evn ^= 1;
    if (evn)
        if (notnull(t->l) && getx(t->l->l) == start)
            t = zigzig(t, 0);
        else
        if (notnull(t->l) && getx(t->l->r) == start)
            t = zigzag(t, 0);
        else
        if (notnull(t->r) && getx(t->r->l) == start)
            t = zigzag(t, 1);
        else
        if (notnull(t->r) && getx(t->r->r) == start)
            t = zigzig(t, 1);
    if (t->p != nullptr)
        return kdsplay(t->p, start, evn, ppval);
    if (getx(t->l) == start)
        t = zig(t, 0);
    else
    if (getx(t->r) == start)
        t = zig(t, 1);
    return t;
}

ptr dsplay(ptr t){
    szgetpath = 0;
    getpath(t);
    for (int i = 0; i < szgetpath; i++)
        push(fgetpath[i]);
    int ppval;
    t = kdsplay(t, t->x, 0, ppval);
    t->pp = ppval;
    return t;
}

ptr q[maxn];

ptr expose(ptr t){
    ptr s;
    while (1){
        t = dsplay(t);
        if (t->pp == -1)
            return t;
        s = q[t->pp];
        s = dsplay(s);
        if (notnull(s->r)){
            s->r->pp = s->x;
            s->r->p = nullptr;
        }
        s->r = t;
        t->p = s;
        s = rec(s);
    }
}

ptr makeroot(ptr t){
    t = expose(t);
    if (notnull(t->l)){
        t->l->p = nullptr;
        t->l->pp = t->x;
        t->l->rev ^= 1;
        push(t->l);
        t->l = nullptr;
    }
    t = rec(t);
    return t;
}

int getminel(ptr &t){
    push(t);
    if (t->l != nullptr)
        return getminel(t->l);
    return t->x;

}

int getroot(ptr &t){
    t = dsplay(t);
    int x = getminel(t);
    q[x] = dsplay(q[x]);
    return x;
}

void link(int v, int u){
    q[u] = makeroot(q[u]);
    q[u]->pp = v;
}

void cut(int v, int u){
    q[v] = makeroot(q[v]);
    q[u] = expose(q[u]);
    q[v]->p = nullptr;
    q[v]->pp = -1;
    q[u]->l = nullptr;
    q[u] = rec(q[u]);
}

int get(int v, int u){
    if (v == u)
        return 0;
    q[v] = makeroot(q[v]);
    q[u] = expose(q[u]);
    if (q[v]->p == nullptr)
        return -1;
    return sz(q[u]->l);
}
