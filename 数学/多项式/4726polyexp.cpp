#include <bits/stdc++.h>
using namespace std;

#define IL inline
#define ri register int 
#define dbg1(x) cout << #x << " = " << x << ", "
#define dbg2(x) cout << #x << " = " << x << endl

template<typename Tp> IL void read(Tp &x) {
    x=0; int f=1; char ch=getchar();
    while(!isdigit(ch)) {if(ch == '-') f=-1; ch=getchar();}
    while(isdigit(ch)) {x =  x*10+ch-'0'; ch=getchar();}
    x *= f;
}
int buf[22];
template<typename Tp> IL void write(Tp x) {
    int p = 0;
    if(x < 0) {putchar('-'); x=-x;}
    if(x == 0) buf[++p] = 0;
    else while(x) {
        buf[++p] = x % 10;
        x /= 10;
    }
    for(int i=p;i;i--) putchar('0' + buf[i]);
}

const int mod = 998244353;
const int G = 3;
const int N = 262144;

IL int ksm(int a, int b, int m) {
    int ret = 1;
    while(b) {
        if(b&1) ret = 1ll * ret * a % m;
        a = 1ll * a * a % m;
        b >>= 1;
    }
    return ret;
}
IL int inv(int x) {return ksm(x, mod-2, mod);}

int rev[N], eps[N], ieps[N];
IL void initeps() {
    int g = ksm(G, (mod-1) / N, mod);
    ieps[0] = eps[0] = 1;
    for(int i=1;i<N;i++) eps[i] = 1ll * eps[i-1] * g % mod;
    for(int i=1;i<N;i++) ieps[i] = eps[N-i];
}

IL void trans(int *x, int *w, int n) {
    for(int i=0;i<n;i++) if(i < rev[i]) swap(x[i], x[rev[i]]);
    for(int i=2;i<=n;i<<=1) {
        int l = i >> 1, d = N / i;
        for(int j=0;j<n;j+=i) {
            for(int k=0;k<l;k++) {
                int t = 1ll * x[j+k+l] * w[d*k] % mod;
                x[j+k+l] = (x[j+k] - t + mod) % mod;
                x[j+k] = (x[j+k] + t) % mod;
            }
        }
    }
}

IL void dft(int *x, int n) { trans(x, eps, n);}
IL void idft(int *x, int n) { 
    trans(x, ieps, n);
    int in = inv(n);
    for(int i=0;i<n;i++) x[i] = 1ll * x[i] * in % mod;
}

int ntt_a[N], ntt_b[N], ntt_c[N];
IL void polyinv(int *B, int *A, int degA) { // use ntt_a
    if(degA == 0) {B[0] = inv(A[0]); B[1] = 0; return;}
    polyinv(B, A, degA >> 1);
    int lim = 1, lglim = 0;
    while(lim <= (degA<<1)) { lim <<= 1; lglim++;}
    for(int i=0;i<lim;i++) rev[i] = (rev[i>>1]>>1) | ((i&1) << (lglim-1));
    for(int i=0;i<=degA;i++) ntt_a[i] = A[i];
    for(int i=degA+1;i<lim;i++) ntt_a[i] = B[i] = 0;
    dft(ntt_a, lim); dft(B, lim);
    for(int i=0;i<lim;i++) B[i] = 1ll*(2-1ll*ntt_a[i]*B[i]%mod+mod)%mod*B[i]%mod;
    idft(B, lim);
    for(int i=degA+1;i<lim;i++) B[i] = 0;
}

IL void polyDao(int *B, int *A, int degA) {
    for(int i=0;i<degA;i++) B[i] = 1ll * (i+1) * A[i+1] % mod; B[degA] = 0;
}
IL void polyJifen(int *B, int *A, int degA) {
    for(int i=0;i<degA;i++) B[i+1] = 1ll * A[i] * inv(i+1) % mod; B[0] = 0;
}

IL void polyln(int *B, int *A, int degA) { // use ntt_a, ntt_b, ntt_c
    polyDao(ntt_b, A, degA); polyinv(ntt_c, A, degA);
    int lim = 1, lglim = 0;
    while(lim <= (degA << 1)) {lim <<= 1; lglim++;}
    for(int i=0;i<lim;i++) rev[i] = (rev[i>>1] >> 1) | ((i&1) << (lglim-1));
    for(int i=degA+1;i<lim;i++) ntt_b[i] = 0;
    for(int i=degA+1;i<lim;i++) ntt_c[i] = 0;
    dft(ntt_b, lim); dft(ntt_c, lim);
    for(int i=0;i<lim;i++) ntt_b[i] = 1ll * ntt_b[i] * ntt_c[i] % mod;
    idft(ntt_b, lim);
    polyJifen(B, ntt_b, degA);
}

int ntt_d[N];
IL void polyexp(int *B, int *A, int degA) {
    if(degA == 0) { B[0] = 1; B[1] = 0; return;}
    polyexp(B, A, degA >> 1);
    polyln(ntt_d, B, degA); // ntt_d = ln(B);
    int lim = 1, lglim = 0;
    while(lim <= (degA << 1)) { lim <<= 1; lglim++;}
    for(int i=0;i<lim;i++) rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (lglim - 1));
    for(int i=0;i<=degA;i++) ntt_d[i] = A[i] >= ntt_d[i] ? A[i] - ntt_d[i] : A[i] - ntt_d[i] + mod;
    ntt_d[0] = ntt_d[0] + 1 >= mod ? 0 : ntt_d[0] + 1;
    for(int i=degA+1;i<lim;i++) ntt_d[i] = B[i] = 0;
    dft(ntt_d, lim); dft(B, lim);
    for(int i=0;i<lim;i++) B[i] = 1ll * ntt_d[i] * B[i] % mod;
    idft(B, lim);
    for(int i=degA+1;i<lim;i++) B[i] = 0;
}

int n;
int f[N], ans[N];

int main() {
    initeps();
    read(n); n--;
    for(int i=0;i<=n;i++) read(f[i]);
    polyexp(ans, f, n);
    for(int i=0;i<=n;i++) {write(ans[i]); putchar(" \n"[i==n]);}
    return 0;
}