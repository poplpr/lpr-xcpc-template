#include<cstdio>
#include<cstring>
#include<cctype>
#include<algorithm>
using namespace std;
typedef long long LL;
#define IL inline
#define ri register int

IL void read(int &x) {
	int f=1; x=0; char ch = getchar();
	while(!isdigit(ch)) {if(ch=='-')f=-1;ch=getchar();}
	while(isdigit(ch)) {x=x*10+ch-'0'; ch=getchar();}
	x *= f;
}

const int N = 2e5 + 3;

int n,m,totrt = 0, totrab = 0, totnode = 0;
int rt[N];
int rab[N<<5],ls[N<<5],rs[N<<5];
LL numv[N<<5];

IL int newnode() { return totrab ? rab[totrab--] : ++totnode;}
IL void del(int o) { rab[++totrab] = o; ls[o]=rs[o]=numv[o]=0; return;}

int upd(int o,int L,int R,int pos,int v) {
	if(!o) o = newnode();
	numv[o] += v;
	if(L == R) { return o;}
	int M = L+R >> 1;
	if(pos <= M) ls[o] = upd(ls[o],L,M,pos,v);
	else rs[o] = upd(rs[o],M+1,R,pos,v);
	return o;
}

LL query(int o,int L,int R,int qL,int qR) {
	if(!o) return 0;
	if(qL <= L && R <= qR) return numv[o];
	int M = L+R >> 1;
	LL ans = 0;
	if(qL <= M) ans += query(ls[o],L,M,qL,qR);
	if(M < qR) ans += query(rs[o],M+1,R,qL,qR);
	return ans;
}

int merge(int u,int v,int L,int R) {
	if(!u||!v) return u|v;
	numv[u] += numv[v];
	if(L == R) return u;
	int M = L+R >> 1;
	ls[u] = merge(ls[u],ls[v],L,M);
	rs[u] = merge(rs[u],rs[v],M+1,R);
	del(v);
	return u;
}

int kth(int o,int L,int R,int k) {
	if(L == R) return L;
	int M = L+R >> 1;
	if(numv[ls[o]] >= k) return kth(ls[o],L,M,k);
	else return kth(rs[o],M+1,R,k-numv[ls[o]]);
}

int split(int u,LL k,int L,int R) {
	int root = newnode();
	int M = L+R >> 1;
	if(k > numv[ls[u]]) rs[root] = split(rs[u],k-numv[ls[u]],L,M);
	else swap(rs[u],rs[root]);
	if(k < numv[ls[u]]) ls[root] = split(ls[u],k,M+1,R);
	numv[root] = numv[u] - k;
	numv[u] = k;
//	if(L == 1 && R == n) {
//		printf("\nnumv[root]=%d,numv[u]=%d\n",numv[root],numv[u]);
//	}
	return root;
} // <= k -> the tree of u, > k -> the tree of root.

int main() {
//	freopen("data.in","r",stdin);
//	freopen("5494.out","w",stdout);
	totnode = totrab = 0; totrt = 1;
	read(n); read(m);
	for(ri i=1;i<=n;i++) {
		int x; read(x);
		rt[1] = upd(rt[1],1,n,i,x);
	}
	while(m--) {
		int op,x,y,z;
		read(op);
		if(op == 0) {
			read(x); read(y); read(z);
			LL k1 = query(rt[x],1,n,1,z), k2 = query(rt[x],1,n,y,z);
			int tmp;
			rt[++totrt] = split(rt[x],k1-k2,1,n);
			tmp = split(rt[totrt],k2,1,n);
			rt[x] = merge(rt[x],tmp,1,n);
		}
		else if(op == 1) {
			read(x); read(y);
			rt[x] = merge(rt[x],rt[y],1,n);
		}
		else if(op == 2) {
			read(x); read(y); read(z);
			rt[x] = upd(rt[x],1,n,z,y);
		}
		else if(op == 3) {
			read(x); read(y); read(z);
			printf("%lld\n",query(rt[x],1,n,y,z));
		}
		else if(op == 4) {
			read(x); read(y);
			if(numv[rt[x]] < y) printf("-1\n"); 
			else printf("%d\n",kth(rt[x],1,n,y));
		}
	}
	return 0;
} 
