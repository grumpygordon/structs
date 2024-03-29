//leftist heap (right subtree has height less than left subtree)

//it is persistent ds, creates node each time
//remove new from merge for non persistence
//add new to merge if you want persistence in some cases


struct X {
    ll w;
    int to;

};
bool operator<(X a, X b) {
    return a.w < b.w;
}

struct heap;

using ptr = heap*;

struct heap {
    X w;
    int h;
    ptr l = nullptr, r = nullptr;
    heap(X w_) {
        w = w_;
        h = 1;
    }
};

typedef heap* ptr;

ptr create(X w) {
    return new heap(w);
}

ptr merge(ptr t1, ptr t2) {
    if (t1 == nullptr)
        return t2;
    if (t2 == nullptr)
        return t1;
    if (t2->w < t1->w)
        swap(t1, t2);
    ptr z = new heap(*t1);
    z->r = merge(t1->r, t2);
    if (z->l == nullptr || z->l->h < z->r->h)
        swap(z->l, z->r);
    z->h = 1 + (z->r == nullptr ? 0 : z->r->h);
    return z;
}

ptr pop(ptr t) {
    if (t != nullptr)
        return merge(t->l, t->r);
    return nullptr;
}

ptr push(ptr t, X w) {
    return merge(t, create(w));
}
