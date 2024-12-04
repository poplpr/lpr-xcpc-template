

#include<algorithm>
#include<cstdio>
#define Z(o)(o->i->s+1)
#define M (i+j>>1)
using std::swap;
struct buf{
	char z[1<<21],*s;
	char e[1<<20],*p;
	buf():s(z),p(e){
		z[fread(z,1,sizeof z,stdin)]=0;
	}
	~buf(){fwrite(e,1,p-e,stdout);}
	operator int(){
		int x=0;
		while(*s<48)++s;
		while(*s>32)
			x=x*10+*s++-48;
		return x;
	}
	void out(int x){
		static char z[12];
		char*i=z;
		if(!x)*p++=48;
		else{
			while(x){
				int y=x/10;
				*i++=x-y*10+48,x=y;
			}
			while(i!=z)*p++=*--i;
		}
		*p++=32;
	}
}it; 
const int N=1e5+5;
typedef struct node*ptr;
struct node{
	int v,s;
	ptr i,j;
	bool r;
	ptr up(){s=i->s+1+j->s;return this;}
	void down(){
		if(r)i->r^=1,j->r^=1,swap(i,j),r=0;
	}
}e[N];
ptr a=e;
ptr get(int v,int s,ptr i,ptr j){
	return&(*++a=(node){v,s,i,j});
}
ptr pre(int i,int j){
	return i>j?e:get(M,j-i+1,pre(i,M-1),pre(M+1,j));
}
void zig(ptr&o,ptr&s){ptr t=o->i;o->i=s,s=o->up(),o=t;}
void zag(ptr&o,ptr&s){ptr t=o->j;o->j=s,s=o->up(),o=t;}
ptr splay(int v,ptr&o){
	ptr s=e,t=e;
	while(1){
		o->down();
		if(v==Z(o))break;
		if(v<Z(o)){
			o->i->down();
			if(v<Z(o->i))
				zig(o,o->i->j);
			zig(o,s);
		}else{
			v-=Z(o);
			o->j->down();
			if(v>Z(o->j))
				v-=Z(o->j),zag(o,o->j->i);
			zag(o,t);
		}
	}
	while(s!=e)zig(s,o->j);
	while(t!=e)zag(t,o->i);
	return o->up();
}
ptr&splay(int s,int t,ptr&o){
	splay(s,o);
	return splay(t-s+2,o->j)->i;
}
void out(ptr o){
	if(o!=e){
		o->down();
		out(o->i);
		it.out(o->v);
		out(o->j);
	}
}
int main(){
	int n=it,m=it;
	ptr r=pre(0,n+1);
	while(m--){int s=it;splay(s,it,r)->r^=1;}
	out(splay(1,n,r));
}

