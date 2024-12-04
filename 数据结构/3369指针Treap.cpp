#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<algorithm>
using namespace std;
#define IL inline 
typedef long long LL;
const int N = 1e5 + 3;

struct Node *null; 
struct Node {
	Node *ch[2];
	int v,sz,r,cnt;
	IL Node() {}
	IL Node(int v):v(v){ ch[0] = ch[1] = null; r = rand(); sz = cnt = 1;}
	IL bool operator < (const Node& rhs) const { return r < rhs.r;}
	IL int cmp(int x) const {
		if(x == v) return -1;
		return x > v;
	}
	IL void upd() { sz = ch[0]->sz + ch[1]->sz + cnt;}
};
IL void initnull() { null = new Node();null->v=null->sz=null->r=null->cnt=0;}
IL void rotate(Node* &o, int d) {
	Node *k = o->ch[d^1]; o->ch[d^1] = k->ch[d]; k->ch[d] = o;
	o->upd(); k->upd(); o = k;
}
void insert(Node* &o, int x) {
	if(o == null) { o = new Node(x); return;}
	o->sz++;
	int d = o->cmp(x);
	if(d == -1) {o->cnt++; return;}
	insert(o->ch[d],x);
	if(o->ch[d]->r > o->r) rotate(o,d^1);
	o->upd();
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

int query_rank(Node *o,int x) {
	if(o == null) return 0;
	if(o->v == x) return o->ch[0]->sz + 1;
	else if(x > o->v)
		return o->ch[0]->sz + o->cnt + query_rank(o->ch[1],x);
	else return query_rank(o->ch[0],x);	
} 

int query_kth(Node *o,int x) {
	if(o == null) return 0;
	if(x <= o->ch[0]->sz) return query_kth(o->ch[0],x);
	else if(x > o->ch[0]->sz + o->cnt) 
		return query_kth(o->ch[1],x - o->ch[0]->sz - o->cnt);
	return o->v;
}

Node *ans, *root;
void query_pre(Node* o,int x) {
	if(o == null) return;
	if(o->v < x) {
		ans = o; query_pre(o->ch[1],x);
	}
	else query_pre(o->ch[0],x);
}
void query_sub(Node* o,int x) {
	if(o == null) return;
	if(o->v > x) {
		ans = o; query_sub(o->ch[0],x);
	}
	else query_sub(o->ch[1],x);
}
IL void ins(int x) {insert(root,x);}
IL void del(int x) {erase(root,x);}
IL int rank(int x) {return query_rank(root,x);}
IL int kth(int x) {return query_kth(root,x);}
IL int pre(int x) {ans=null;query_pre(root,x);return ans->v;}
IL int sub(int x) {ans=null;query_sub(root,x);return ans->v;}

IL void init() { initnull(); ans = root = null;}

int main() {
	int n; scanf("%d",&n);
    int op,x;
    init();
    for(int i=0;i<n;i++) {
        scanf("%d%d",&op,&x);
        switch(op) {
            case 1: ins(x); break;
            case 2: del(x); break;
            case 3: printf("%d\n",rank(x)); break;
            case 4: printf("%d\n",kth(x)); break;
            case 5: printf("%d\n",pre(x)); break;
            case 6: printf("%d\n",sub(x)); break;
        }
    }
	return 0;
} 
