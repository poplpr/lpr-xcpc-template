#include<cstdio>
#include<cstring>
#include<vector>
#include<algorithm>
using namespace std;
#define IL inline 
#define ri register int
typedef long long LL;

struct Node *null;
struct Node {
	Node *ch[2];
	int v,sz,cnt,flip;
	IL Node() {}
	IL Node(int v):v(v){ch[0]=ch[1]=null;flip=0;sz=cnt=1;}
	IL int cmp(int k) const {
		int d = k - ch[0]->sz;
		if(d == 1) return -1;
		return d > 0;
	}
	IL void upd() { sz = cnt + ch[0]->sz + ch[1]->sz;}
	IL void pushdown() {
		if(flip) {
			flip = 0;
			swap(ch[0],ch[1]);
			ch[0]->flip ^= 1;
			ch[1]->flip ^= 1;
		}
	}
};
IL void initnull() { null = new Node(); null->sz = null->v = null->cnt = null->flip = 0;}
IL void rotate(Node *&o,int d) {
	Node* k = o->ch[d^1]; o->ch[d^1] = k->ch[d]; k->ch[d] = o;
	o->upd(); k->upd(); o = k;
}
void splay(Node* &o,int k) {
	o->pushdown();
	int d = o->cmp(k);
	if(d == 1) k -= o->ch[0]->sz + o->cnt;
	if(d != -1) {
		Node* p = o->ch[d];
		p->pushdown(); 
		int d2 = p->cmp(k);
		int k2 = (d2 == 0 ? k : k - p->ch[0]->sz - o->cnt);
		if(d2 != -1) {
			splay(p->ch[d2],k2);
			if(d == d2) rotate(o,d^1); else rotate(o->ch[d],d);
		}
		rotate(o,d^1);
	}
}
IL Node* merge(Node* left,Node* right) {
	splay(left,left->sz);
	left->ch[1] = right;
	left->upd();
	return left;
}
IL void split(Node* o,int k,Node *&left, Node *&right) {
	splay(o,k);
	left = o;
	right = o->ch[1];
	o->ch[1] = null;
	left->upd(); 
}

const int N = 1e5 + 9;
int n,m,valcnt;
int val[N];
Node *root;

IL Node* build(int sz) {
	if(!sz) return null;
	Node *l = build(sz/2);
	Node *o = new Node(val[++valcnt]);
	o->ch[0] = l; o->ch[1] = build(sz-sz/2-1);
	o->upd();
	return o;
}

IL void init(int sz) {
	initnull(); root = null;
	for(int i=0;i<=n+2;i++) val[i] = i;
	valcnt = 0;
	root = build(sz);
}

vector<int> ans;
void print(Node *o) {
	if(o == null) return;
	o->pushdown();
	print(o->ch[0]);
	ans.push_back(o->v);
	print(o->ch[1]);
}

int main() {
	scanf("%d%d",&n,&m);
	init(n+2);
	while(m--) {
		int a,b; scanf("%d%d",&a,&b);
		Node *left,*mid,*right,*o;
		split(root,a,left,o);
		split(o,b-a+1,mid,right);
		mid->flip ^= 1;
		root = merge(merge(left,mid),right);
	}
	ans.clear();
	print(root);
	for(int i=1;i<ans.size()-1;i++) printf("%d ",ans[i]-1);
	return 0;
}
