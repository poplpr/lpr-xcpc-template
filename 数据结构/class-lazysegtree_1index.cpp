namespace LPR {

template <class S,
          S (*op)(S, S),
          S (*e)(),
          class F,
          S (*mapping)(F, S),
          F (*composition)(F, F),
          F (*id)()>
struct lazy_segtree_1index {
  public:
    lazy_segtree_1index() : lazy_segtree_1index(0) {}
    explicit lazy_segtree_1index(int n) : lazy_segtree_1index(std::vector<S>(n, e())) {}
    explicit lazy_segtree_1index(const std::vector<S>& v) : _n(int(v.size())) {
        size = 4 * _n;
        d = std::vector<S>(size, e());
        lz = std::vector<F>(size, id());
        _build(1, 1, _n, v);
    }
    void _build(int o, int L, int R, const std::vector<S>& v) {
        if (L == R) { d[o] = v[L-1]; return;}
        int M = L+R >> 1;
        _build(o << 1, L, M, v); _build(o << 1 | 1, M + 1, R, v);
        pushup(o, L, R);
    }

    int _pos; S _val;
    void _set(int o, int L, int R) {
        if (L == R) { d[o] = _val; return;}
        pushdown(o, L, R);
        int M = L+R >> 1;
        if (_pos <= M) _set(o << 1, L, M);
        else _set(o << 1 | 1, M + 1, R);
        pushup(o, L, R);
    }
    void set(int p, S x) {
        assert(1 <= p && p <= _n);
        _pos = p; _val = x;
        _set(1, 1, _n);
    }

    S _get(int o, int L, int R) {
        if (L == R) return d[o];
        pushdown(o, L, R);
        int M = L+R >> 1;
        S ret = e();
        if (_pos <= M) ret = _get(o << 1, L, M);
        else ret = _get(o << 1 | 1, M + 1, R);
        pushup(o, L, R);
        return ret;
    }
    S get(int p) {
        assert(1 <= p && p <= _n);
        _pos = p;
        return _get(1, 1, _n);
    }

    int qL, qR;
    S _query(int o, int L, int R) {
        if (qL <= L && R <= qR) return d[o];
        pushdown(o, L, R);
        int M = L+R >> 1;
        S ret = e();
        if (qL <= M && M >= qR) ret = _query(o << 1, L, M);
        if (qL <= M && M < qR) ret = op(_query(o << 1, L, M), _query(o << 1 | 1, M + 1, R));
        pushup(o, L, R);
        return ret;
    }
    S prod(int l, int r) {
        assert(1 <= l && l <= r && r <= _n);
        qL = l; qR = r;
        return _query(1, 1, _n);
    }

    S all_prod() const { return d[1]; }

    F _valF;
    void apply(int p, F f) {
        assert(1 <= p && p <= _n);
        qL = qR = p;
        _valF = f;
        _apply(1, 1, _n);
    }
    void _apply(int o, int L, int R) {
        if (qL <= L && R <= qR) {
            all_apply(o, _valF);
            return;
        }
        pushdown(o, L, R);
        int M = L+R >> 1;
        if (qL <= M) _apply(o << 1, L, M);
        if (M < qR) _apply(o << 1 | 1, M + 1, R);
        pushup(o, L, R);
    }
    void apply(int l, int r, F f) {
        dbg(l, r);
        assert(1 <= l && l <= r && r <= _n);
        qL = l; qR = r; _valF = f;
        _apply(1, 1, _n);
    }

  private:
    int _n, size, log;
    std::vector<S> d;
    std::vector<F> lz;

    void pushup(const int& o, const int& L, const int& R) { 
        dbg(o, L, R, d[o << 1].minx, d[o << 1 | 1].minx, d[o].minx);
        d[o] = op(d[o << 1], d[o << 1 | 1]); 
    }
    void all_apply(int k, F f) {
        d[k] = mapping(f, d[k]);
        lz[k] = composition(f, lz[k]);
    }
    void pushdown(const int& o, const int& L, const int& R) {
        all_apply(o << 1, lz[o]);
        all_apply(o << 1 | 1, lz[o]);
        lz[o] = id();
    }
};

}