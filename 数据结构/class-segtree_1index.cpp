namespace LPR {

template <class S, S (*op)(S, S), S (*e)()> struct segtree_1index {
// from 1-index, query and modify segment [L, R].
  public:
    segtree_1index() : segtree_1index(0) {}
    explicit segtree_1index(int n) : segtree_1index(std::vector<S>(n, e())) {}
    explicit segtree_1index(const std::vector<S>& v) : _n(int(v.size())) {
        size = 4 * _n;
        d = std::vector<S>(size, e());
        _build(1, 1, _n, v);
    }
    void _build(int o, int L, int R, const std::vector<S>& v) {
        if (L == R) { d[o] = v[L-1]; return;}
        int M = L+R >> 1;
        _build(o << 1, L, M, v); _build(o << 1 | 1, M + 1, R, v);
        pushup(o);
    }

    int _pos; S _val;
    void _modify(int o, int L, int R) {
        if (L == R) { d[o] = _val; return;}
        int M = L+R >> 1;
        if (_pos <= M) _modify(o << 1, L, M);
        else _modify(o << 1 | 1, M + 1, R);
        pushup(o);
    }
    void set(int p, S x) {
        assert(1 <= p && p <= _n);
        _pos = p; _val = x;
        _modify(1, 1, _n);
    }

    S _get(int o, int L, int R) {
        if (L == R) return d[o];
        int M = L+R >> 1;
        if (_pos <= M) return _get(o << 1, L, M);
        else return _get(o << 1 | 1, M + 1, R);
    }
    S get(int p) {
        assert(1 <= p && p <= _n);
        _pos = p;
        return _get(1, 1, _n);
    }

    int qL, qR;
    S _query(int o, int L, int R) {
        if (qL <= L && R <= qR) return d[o];
        int M = L+R >> 1;
        if (qL <= M && M >= qR) return _query(o << 1, L, M);
        if (qL <= M && M < qR) return op(_query(o << 1, L, M), _query(o << 1 | 1, M + 1, R));
    }
    S prod(int l, int r) {
        assert(1 <= l && l <= r && r <= _n);
        qL = l; qR = r;
        return _query(1, 1, _n);
    }

    S all_prod() const { return d[1]; }

  private:
    int _n, size;
    std::vector<S> d;

    void pushup(int o) {d[o] = op(d[o << 1], d[o << 1 | 1]);}
};

}