#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
typedef long long LL;
#define IL inline
#define ri register int 
#define mk make_pair

const int N = 19;
const int MOD = 998244353;
const int inv2 = 499122177;

int n;
int a[1<<N],b[1<<N];
int ora[1<<N],orb[1<<N],orc[1<<N];
int anda[1<<N],andb[1<<N],andc[1<<N];
int xora[1<<N],xorb[1<<N],xorc[1<<N];

IL void OR(int *A,int n,int x) {
	for(ri i=1;i<(1<<n);i<<=1) {
		for(ri p=i<<1,j=0;j<(1<<n);j+=p) {
			for(ri k=0;k<i;k++) {
				A[i+j+k] = (1LL * A[i+j+k] + A[j+k] * x + MOD) % MOD;
			}
		}
	}
} 

IL void AND(int *A,int n,int x) {
	for(ri i=1;i<(1<<n);i<<=1) {
		for(ri p=i<<1,j=0;j<(1<<n);j+=p) {
			for(ri k=0;k<i;k++) {
				A[j+k] = (1LL * A[j+k] + A[i+j+k]*x + MOD) % MOD;
			}
		}
	}
}

IL void XOR(int *A,int n,int t) {
	for(ri i=1;i<(1<<n);i<<=1) {
		for(ri p=i<<1,j=0;j<(1<<n);j+=p) {
			for(ri k=0;k<i;k++) {
				int x = A[j+k], y = A[i+j+k];
				A[j+k] = (1LL*x+y) % MOD;
				A[i+j+k] = (1LL*x-y+MOD) % MOD;
				if(t == -1) {
					A[j+k] = 1LL * A[j+k] * inv2 % MOD;
					A[i+j+k] = 1LL * A[i+j+k] * inv2 % MOD;
				}
			}
		}
	}
}

int main() {
	scanf("%d",&n);
	for(ri i=0;i<(1<<n);i++) scanf("%d",a+i);
	for(ri i=0;i<(1<<n);i++) scanf("%d",b+i);
	memcpy(ora,a,sizeof(int)*(1<<n));
	memcpy(orb,b,sizeof(int)*(1<<n));
	memcpy(anda,a,sizeof(int)*(1<<n));
	memcpy(andb,b,sizeof(int)*(1<<n));
	memcpy(xora,a,sizeof(int)*(1<<n));
	memcpy(xorb,b,sizeof(int)*(1<<n));
	OR(ora,n,1); OR(orb,n,1);
	for(ri i=0;i<(1<<n);i++) orc[i] = 1LL*ora[i]*orb[i]%MOD;
	OR(orc,n,-1);
	AND(anda,n,1); AND(andb,n,1);
	for(ri i=0;i<(1<<n);i++) andc[i] = 1LL*anda[i]*andb[i]%MOD;
	AND(andc,n,-1);
	XOR(xora,n,1); XOR(xorb,n,1);
	for(ri i=0;i<(1<<n);i++) xorc[i] = 1LL*xora[i]*xorb[i]%MOD;
	XOR(xorc,n,-1);
	for(ri i=0;i<(1<<n);i++) printf("%d%c",orc[i]," \n"[i==(1<<n)-1]);
	for(ri i=0;i<(1<<n);i++) printf("%d%c",andc[i]," \n"[i==(1<<n)-1]);
	for(ri i=0;i<(1<<n);i++) printf("%d%c",xorc[i]," \n"[i==(1<<n)-1]);
	return 0;
}
