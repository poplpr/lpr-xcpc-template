#include<cstdio>
#include<cstring>
#include<cctype>
#include<cstdlib>
#include<algorithm>
using namespace std;
typedef long long LL;
#define IL inline
#define ri register int
IL void read(int &x) {
	x=0; int f=1; char ch=getchar();
	while(!isdigit(ch)) {if(ch=='-')f=-1;ch=getchar();}
	while(isdigit(ch)) {x=x*10+ch-'0';ch=getchar();}
	x *= f;
}
int buf[23];
IL void write(int i) {
	int p = 0;
	if(i < 0) {putchar('-'); i=-i;}
	if(i == 0) {buf[p++] = 0;}
	else while(i) {
		buf[p++] = i % 10;
		i /= 10;
	}
	for(ri j=p-1;j>=0;j--) putchar(buf[j] + '0');
}

struct Node *null;
struct Node {
	Node *ch[2];
	int v,sz,r,cnt;
	IL Node() {}
	IL Node(int v):v(v) {ch[0]=ch[1]=null; r=rand(); sz=cnt=1;}
	IL bool operator < (const Node& rhs) const { return r < rhs.r;}
	IL int cmp(int x) const {
		if(x == v) return -1;
		return v < x;
	}
	IL void upd() { sz = ch[0] -> sz + ch[1] -> sz + cnt;}
};

IL void initnull() {null = new Node(); null->v=null->sz=null->r=null->cnt=0;}
IL void rotate(Node* &o,int d) {
	Node *k = o->ch[d^1]; o->ch[d^1] = k->ch[d]; k->ch[d] = o;
	o->upd(); k->upd(); o = k;
}
void insert(Node* &o,int x) {
	if(o == null) {o = new Node(x); return;}
	o->sz++;
	int d = o->cmp(x);
	if(d == -1) {o->cnt++; return;}
	insert(o->ch[d],x);
	if(o->r < o->ch[d]->r) rotate(o,d^1);
	o -> upd();
}
void erase(Node* &o,int x) {
	if(o == null) return;
	int d = o->cmp(x);
	if(d == -1) {
		Node* u = o;
		if(o->cnt > 1) {o->cnt--; o->sz--; return;}
		if(o->ch[0] != null && o->ch[1] != null) {
			int d2 = o->ch[0]->r > o->ch[1]->r;
			rotate(o,d2); erase(o->ch[d2],x);
		}
		else {
			if(o->ch[0] == null) o = o->ch[1]; else o = o->ch[0];
			delete u;
		}
	}
	else erase(o->ch[d],x);
	if(o != null) o->upd();
}
int query_rank(Node *o, int x) {
	if(o == null) return 1;
	if(o->v == x) return o->ch[0]->sz + 1;
	else if(o->v < x) return o->ch[0]->sz + o->cnt + query_rank(o->ch[1],x);
	else return query_rank(o->ch[0],x);
}
int query_kth(Node* o, int k) {
	if(o == null) return 0;
	if(k <= o->ch[0]->sz) return query_kth(o->ch[0],k);
	else if(k > o->ch[0]->sz + o->cnt)
		return query_kth(o->ch[1],k - o->ch[0]->sz - o->cnt);
	return o->v;
}

Node *ans,*root;
void query_pre(Node* o,int x) {
	if(o == null) return;
	if(o->v < x) { ans = o; query_pre(o->ch[1],x);}
	else query_pre(o->ch[0],x);
}
void query_sub(Node* o,int x) {
	if(o == null) return;
	if(x < o->v) { ans = o; query_sub(o->ch[0],x);}
	else query_sub(o->ch[1],x);
}

const int N = 1e5 + 5;

int n,m;

int main() {
	read(n); read(m);
	initnull();
	ans = root = null;
	for(ri i=1;i<=n;i++) {
		int a; read(a);
		insert(root,a);
	}
	int last = 0, ansi = 0;
	while(m--) {
		int op,x; read(op); read(x); x ^= last;
		if(op == 1) insert(root,x);
		else if(op == 2) erase(root,x);
		else if(op == 3) {last=query_rank(root,x);}
		else if(op == 4) {last=query_kth(root,x);}
		else if(op == 5) {query_pre(root,x); last = ans -> v ;}
		else if(op == 6) {query_sub(root,x); last = ans -> v ;}
		if(op != 1 && op != 2) ansi ^= last;
	}
	printf("%d\n",ansi);
	return 0;
}
