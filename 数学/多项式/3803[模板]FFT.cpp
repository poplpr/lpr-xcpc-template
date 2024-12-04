#include<cstdio>
#include<cstring>
#include<algorithm>
#include<cmath>
using namespace std;
typedef double db;
#define IL inline
#define ri register int 

const db PI = acos(-1.0);

// 虚数部分
struct com {
	db x,y;
	com(db x=0.0,db y=0.0):x(x),y(y) {}
};
IL com operator + (const com& a,const com& b) { return com(a.x+b.x,a.y+b.y);}
IL com operator - (const com& a,const com& b) { return com(a.x-b.x,a.y-b.y);}
IL com operator * (const com& a,const com& b) { return com(a.x*b.x-a.y*b.y, a.x*b.y+a.y*b.x);}
IL com operator * (const com& a,const db& p) { return com(a.x*p, a.y*p);}
IL com operator / (const com& a,const db& p) { return com(a.x/p, a.y/p);}
IL com polar(db rho,db theta) { return com(rho*cos(theta), rho*sin(theta));}
IL com operator !(const com& a) {return com(a.x, -a.y);}

// 快速傅里叶变换部分
const int N = 262144; // 要开到给定的 2 倍以上，且必须是 2 的整数次幂的形式。
struct buf{
	char a[1<<25],*s;
	char b[1<<25],*t;
	buf():s(a),t(b){a[fread(a,1,sizeof a,stdin)]=0;}
	~buf(){fwrite(b,1,t-b,stdout);}
	operator int(){
		int x=0;
		while(*s<48)++s;
		while(*s>32)
			x=x*10+*s++-48;
		return x;
	}
	void out(int x){
		static char c[12];
		char*i=c;
		if(!x)*t++=48;
		else{
			while(x){
				int y=x/10;
				*i++=x-y*10+48,x=y;
			}
			while(i!=c)*t++=*--i;
		}
		*t++=10;
	}
}it;
int rev[N];
com eps[N], ieps[N];
IL void initeps() {
	for(ri i=0;i<N;i++) eps[i] = polar(1.0,2*PI*i/N);
	ieps[0] = eps[0] = 1;
	for(ri i=1;i<N;i++) ieps[i] = eps[N-i];
}
IL void cal_rev(int nA, int nB, int& lim, int& p) {
	lim = 1; p = 0;
	while(lim <= ((nA+nB) >> 1)) {lim <<= 1; ++p;}
	for(ri i=0;i<lim;i++) rev[i] = (rev[i>>1]>>1) | ((i&1)<<(p-1));
}
IL void trans(com *x, com* w, int n) {
	for(ri i=0; i<n; i++) if(i < rev[i]) swap(x[i],x[rev[i]]);
	for(ri i=2;i<=n;i<<=1) {
		int l = i>>1, d = N / i;
		for(ri j=0;j<n;j+=i) {
			for(ri k=0;k<l;k++) {
				com t = x[j+k+l] * w[d*k];
				x[j+k+l] = x[j+k] - t;
				x[j+k] = x[j+k] + t;
			}
		}
	}
}
IL void dft(com* x, int n) { trans(x,eps,n);}
IL void idft(com* x, int n) { trans(x,ieps,n); for(ri i=0;i<n;i++) x[i] = x[i] / n;}

com fft_a[N], fft_b[N], fft_c[N];
IL int mul_normal(db *C, db *A, db *B, int degA, int degB) { // 正常卷积 C = A * B
	int lim, p;
	cal_rev(degA, degB, lim, p); // 如果多项式长度都相等的话，这句可以写在外面
	for(ri i=0;i<=degA;i++) (i&1 ? fft_a[i>>1].y : fft_a[i>>1].x) = A[i];
	for(ri i=0;i<=degB;i++) (i&1 ? fft_b[i>>1].y : fft_b[i>>1].x) = B[i];
	dft(fft_a, lim); dft(fft_b, lim);
	for(ri i=0;i<lim;i++) {
        int j = (lim-1) & (lim - i);
        fft_c[i] = (fft_a[i]*fft_b[i]*4 - (fft_a[i]-!fft_a[j]) * (fft_b[i]-!fft_b[j]) * (eps[N/lim*i] + com(1,0))) * 0.25;
    }
	idft(fft_c, lim);
    for(int i=0;i<=degA+degB;i++) C[i] = (i&1) ? fft_c[i>>1].y : fft_c[i>>1].x;
	return degA + degB; // 返回新多项式的次数.
}

db f[N], g[N], ans[N];

int main() {
	initeps();
	int n,m; n = it; m = it;
	for(ri i=0;i<=n;i++) f[i] = it;
	for(ri i=0;i<=m;i++) g[i] = it;
	mul_normal(ans,f,g,n,m);
	for(ri i=0;i<=n+m;i++) it.out((int)(ans[i] + 0.1));
	return 0;
} 