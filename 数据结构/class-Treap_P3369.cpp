#include <bits/stdc++.h>
using namespace std;

template<class T> class Treap {
public:
	Treap() {}
	~Treap() { _clear(root);}
	void insert(T x) { _insert(root, x);}
	void erase(T x) { _erase(root, x);}
	int rank(T x) { return _GetRankOfVal(root, x);}
	T kth(int x) { assert(1 <= x && x <= root->sz); return _GetValOfRank(root, x);}
	T pre(T x) { Node *ans = null; query_pre(root, x, ans); return ans->v;}
	T nxt(T x) { Node *ans = null; query_nxt(root, x, ans); return ans->v;}
	bool empty() { return root->sz == 0;}
	int size() { return root -> sz;}
	void clear() { _clear(root);}
    void swap(Treap<T>& rhs) { std::swap(root, rhs.root);}

private:
	struct Node {
		Node *ch[2];
		T v;
		int sz, r, cnt;
		Node() { sz = r = cnt = 0;}
		Node(const T &v):v(v) { ch[0] = ch[1] = null; r=rand(); sz = cnt = 1;}
		bool operator < (const Node& rhs) const { return r < rhs.r;}
		int cmp(const T& x) const {
			if(!(x < v || v < x)) return -1;
			return v < x;
		}
		void upd() { sz = ch[0] -> sz + ch[1] -> sz + cnt;}
	};
	static Node *null;
	Node *root = null;

	void rotate(Node* &o, const int &d) {
		Node *k = o->ch[d^1]; o->ch[d^1] = k->ch[d]; k->ch[d] = o;
		o->upd(); k->upd(); o = k;
	}
	void _insert(Node* &o, const T &x) {
		if (o == null) { o = new Node(x); return;}
		o->sz++;
		int d = o->cmp(x);
		if (d == -1) {o->cnt++; return;}
		_insert(o->ch[d], x);
		if (o->r < o->ch[d]->r) rotate(o, d^1);
		o -> upd();
	}
	void _erase(Node* &o, const T &x) {
		if (o == null) return;
		int d = o->cmp(x);
		if (d == -1) {
			Node* u = o;
			if (o->cnt > 1) {o->cnt--; o->sz--; return;}
			if (o->ch[0] != null && o->ch[1] != null) {
				int d2 = o->ch[0]->r > o->ch[1]->r;
				rotate(o, d2); _erase(o->ch[d2], x);
			}
			else {
				if (o->ch[0] == null) o = o->ch[1]; else o = o->ch[0];
				delete u;
			}
		}
		else _erase(o->ch[d], x);
		if(o != null) o->upd();
	}
	int _GetRankOfVal(Node *&o, const T &x) {
		if (o == null) return 1;
		if (!(o->v < x || x < o->v)) return o->ch[0]->sz + 1;
		else if (o->v < x) return o->ch[0]->sz + o->cnt + _GetRankOfVal(o->ch[1], x);
		else return _GetRankOfVal(o->ch[0], x);
	}
	T _GetValOfRank(Node *&o, const int &k) {
		if (o == null) return T();
		if (!(o->ch[0]->sz < k)) return _GetValOfRank(o->ch[0], k);
		else if(o->ch[0]->sz + o->cnt < k)
			return _GetValOfRank(o->ch[1], k - o->ch[0]->sz - o->cnt);
		return o->v;
	}

	void query_pre(Node *&o, const T &x, Node *&ans) {
		if (o == null) return;
		if (o->v < x) { ans = o; query_pre(o->ch[1], x, ans);}
		else query_pre(o->ch[0], x, ans);
	}
	void query_nxt(Node *&o, const T &x, Node *&ans) {
		if (o == null) return;
		if (x < o->v) { ans = o; query_nxt(o->ch[0], x, ans);}
		else query_nxt(o->ch[1], x, ans);
	}
	void _clear(Node*& o) {
		if (o == null || o == NULL) return;
		_clear(o -> ch[0]); 
		_clear(o -> ch[1]);
        delete o;
        return;
	}
};
template<class T> typename Treap<T>::Node* Treap<T>::null = new Node();

struct AAA {
	int a;
	// AAA(int a = 0):a(a) {}
	bool operator < (const AAA& rhs) const {
		return a < rhs.a;
	}
};

int main() {
#ifdef LOCAL
	freopen("test.in", "r", stdin);
#endif
	int n; scanf("%d",&n);
    int op,y;
	Treap<AAA> S;
    for(int i=0;i<n;i++) {
        scanf("%d%d",&op,&y);
		AAA x = AAA{y};
        switch(op) {
            case 1: S.insert(x); break;
            case 2: S.erase(x); break;
            case 3: printf("%d\n",S.rank(x)); break;
            case 4: printf("%d\n",S.kth(y).a); break;
            case 5: printf("%d\n",S.pre(x).a); break;
            case 6: printf("%d\n",S.nxt(x).a); break;
        }
    }
	return 0;
} 
