#include<cstdio>
#include<cstring>
#include<vector>
#include<cctype>
#include<algorithm>
using namespace std;
typedef long long LL;
#define IL inline
#define ri register int
IL void read(int &x) {
	x=0; int f=1; char ch=getchar();
	while(!isdigit(ch)) {if(ch=='-')f=-1;ch=getchar();}
	while(isdigit(ch)) {x=x*10+ch-'0';ch=getchar();}
	x*=f;
}

const int N = 1e5 + 3;

int dfs_clock = 0;
int depth[N],sz[N],son[N],pa[N];
int dfn[N],top[N];
vector<int> G[N];
void dfs1(int u,int fa,int nowd) {
	depth[u] = nowd;
	sz[u] = 1;
	son[u] = 0;
	pa[u] = fa;
	for(ri i=0;i<G[u].size();i++) {
		int v = G[u][i];
		if(v == fa) continue;
		dfs1(v,u,nowd+1);
		sz[u] += sz[v];
		if(sz[v] > sz[son[u]]) son[u] = v;
	}
}
void dfs2(int u,int nowtop) {
	dfn[u] = ++dfs_clock;
	top[u] = nowtop;
	if(son[u]) dfs2(son[u],nowtop);
	for(ri i=0;i<G[u].size();i++) {
		int v = G[u][i];
		if(v == pa[u] || v == son[u]) continue;
		dfs2(v,v);		
	}
}
int lca(int u,int v) {
	while(top[u] != top[v]) {
		if(depth[top[u]] < depth[top[v]]) swap(u,v);
		u = pa[top[u]];
	}
	return depth[u] < depth[v] ? u : v;
}

int totnode = 0;
int rt[N];
int ls[N<<7],rs[N<<7],maxv[N<<7],num[N<<7];
IL void pushup(int o) {
	if(maxv[ls[o]] >= maxv[rs[o]]) {maxv[o]=maxv[ls[o]]; num[o]=num[ls[o]];}
	else {maxv[o]=maxv[rs[o]]; num[o]=num[rs[o]];}
}
int upd(int o,int L,int R,int &pos,int &v) {
	if(!o) o = ++totnode;
	if(L == R) {maxv[o] += v; num[o]=L; return o;}
	int M = L+R >> 1;
	if(pos <= M) ls[o] = upd(ls[o],L,M,pos,v);
	else rs[o] = upd(rs[o],M+1,R,pos,v);
	pushup(o);
	return o;
} 
int merge(int u,int v,int L,int R) {
	if(!u || !v) return u|v;
	if(L == R) {maxv[u] += maxv[v]; num[u] = L; return u;}
	int M = L+R >> 1;
	ls[u] = merge(ls[u],ls[v],L,M);
	rs[u] = merge(rs[u],rs[v],M+1,R);
	pushup(u);
	return u;
}
void debug(int o,int L,int R) {
	if(!o) return;
	printf("[%d,%d],maxv=%d,num=%d\n",L,R,maxv[o],num[o]);
	if(L == R) {return;}
	int M = L+R >> 1;
	debug(ls[o],L,M); debug(rs[o],M+1,R);
}

int n,m,maxz;
int X[N],Y[N],Z[N],ans[N];

void dfs3(int u) {
	for(ri i=0;i<G[u].size();i++) {
		int v = G[u][i];
		if(depth[v] > depth[u]) {
			dfs3(v);
			rt[u] = merge(rt[u],rt[v],1,maxz);
		}
	}
	ans[u] = maxv[rt[u]] ? num[rt[u]] : 0;
}

int main() {
	maxz = totnode = dfs_clock = 0;
	read(n); read(m);
	for(ri i=1;i<n;i++) {
		int u,v; read(u); read(v);
		G[u].push_back(v);
		G[v].push_back(u);
	}
	dfs1(1,0,1);
	dfs2(1,1);
	for(ri i=1;i<=m;i++) {
		read(X[i]); read(Y[i]); read(Z[i]);
		maxz = max(maxz,Z[i]);
	}
	for(ri i=1;i<=m;i++) {
		int LCA = lca(X[i],Y[i]), v=1;
		rt[X[i]]=upd(rt[X[i]],1,maxz,Z[i],v);
		rt[Y[i]]=upd(rt[Y[i]],1,maxz,Z[i],v);
		v = -1;
		rt[LCA]=upd(rt[LCA],1,maxz,Z[i],v);
		rt[pa[LCA]]=upd(rt[pa[LCA]],1,maxz,Z[i],v);
	}
	dfs3(1);
	for(ri i=1;i<=n;i++) printf("%d\n",ans[i]);
	return 0;
}
