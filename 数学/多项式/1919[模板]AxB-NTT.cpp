#include<bits/stdc++.h>
using namespace std;

#define IL inline

const int mod = 998244353;
const int N = 2097152;
const int G = 3;

IL int ksm(int a, int b, int m) {
	int ret = 1;
	while(b) {
		if(b&1) ret = 1ll * ret * a % m;
		a = 1ll * a * a % m;
		b >>= 1;
	}
	return ret;
}
IL int inv(int x) { return ksm(x, mod-2, mod);}

int rev[N];
int eps[N], ieps[N];
IL void initeps() {
	int g = ksm(G, (mod-1) / N, mod), ig = inv(g);
	ieps[0] = eps[0] = 1;
	for(int i=1;i<N;i++) eps[i] = 1ll * eps[i-1] * g % mod;
	for(int i=1;i<N;i++) ieps[i] = 1ll * ieps[i-1] * ig % mod;
}
IL void cal_rev(int degA, int degB, int &lim, int &p) {
	lim = 1; p = 0;
	while(lim < degA + degB) { lim <<= 1; ++p;}
	for(int i=0;i<lim;i++) rev[i] = (rev[i>>1]>>1) | ((i&1) << (p-1));
}
IL void trans(int *x, int *w, int n) {
	for(int i=0;i<n;i++) if(i < rev[i]) swap(x[i], x[rev[i]]);
	for(int i=2;i<=n;i<<=1) {
		int l = i >> 1, d = N / i;
		for(int j=0;j<n;j+=i) {
			for(int k=0;k<l;k++) {
				int t = 1ll * x[j+k+l] * w[d*k] % mod;
				x[j+k+l] = (1ll * x[j+k] - t + mod) % mod;
				x[j+k] = (x[j+k] + t) % mod;
			}
		}
	}
}
IL void dft(int *x, int n) {trans(x, eps, n);}
IL void idft(int *x, int n) {
	trans(x, ieps, n);
	int in = inv(n);
	for(int i=0;i<n;i++) x[i] = 1ll * x[i] * in % mod;
}

int ntt_a[N], ntt_b[N];
IL int mul_normal(int *C, int *A, int *B, int degA, int degB) {
	int lim, p;
	cal_rev(degA, degB, lim, p);
	for(int i=0;i<=degA;i++) ntt_a[i] = A[i];
	for(int i=0;i<=degB;i++) ntt_b[i] = B[i];
	dft(ntt_a, lim); dft(ntt_b, lim);
	for(int i=0;i<lim;i++) ntt_a[i] = 1ll * ntt_a[i] * ntt_b[i] % mod;
	idft(ntt_a, lim);
	for(int i=0;i<=degA+degB;i++) C[i] = ntt_a[i];
	return degA + degB;
}

const int M = 1000000 + 5;

char as[M], bs[M];
int a[M], b[M], ans[N];

int main() {
	initeps();
	scanf("%s", as);
	scanf("%s", bs);
	int degA = strlen(as) - 1, degB = strlen(bs) - 1;
	for(int i=0;i<=degA;i++) a[i] = as[degA-i] - '0';
	for(int i=0;i<=degB;i++) b[i] = bs[degB-i] - '0';
	
	int degans = mul_normal(ans, a, b, degA, degB);
	
	for(int i=0;i<=degans;i++) {
		if(ans[i] >= 10) {
			ans[i+1] += ans[i] / 10;
			ans[i] %= 10;
			if(i == degans) degans++;
		}
	}
	for(int i=degans;i>=0;i--) putchar(ans[i] + '0');
	return 0;
} 
