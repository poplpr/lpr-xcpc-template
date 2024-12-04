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
const int N = 524288;

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
const int inv2 = inv(2);

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

int jc[N], ijc[N];
int ntt_a[N], ntt_b[N], ntt_c[N];
void FFPmul(int *C, int *A, int *B, int degA, int degB) {
    int lim = 1, lglim = 0;
    while(lim <= (degA+degB)*2) {lim <<= 1; lglim++;}
    for(int i=0;i<lim;i++) rev[i] = (rev[i>>1]>>1) | ((i&1) << (lglim-1));

    for(int i=0;i<=degA;i++) ntt_a[i] = A[i];
    for(int i=degA+1;i<lim;i++) ntt_a[i] = 0;
    for(int i=0;i<=degB;i++) ntt_b[i] = B[i];
    for(int i=degB+1;i<lim;i++) ntt_b[i] = 0;
    for(int i=0;i<=degA+degB;i++) ntt_c[i] = ijc[i];
    for(int i=degA+degB+1;i<lim;i++) ntt_c[i] = 0;
    dft(ntt_a, lim); dft(ntt_b, lim); dft(ntt_c, lim);
    for(int i=0;i<lim;i++) {
        ntt_a[i] = 1ll * ntt_a[i] * ntt_c[i] % mod;
        ntt_b[i] = 1ll * ntt_b[i] * ntt_c[i] % mod;
    }
    idft(ntt_a, lim); idft(ntt_b, lim);
    for(int i=0;i<=degA+degB;i++) ntt_a[i] = 1ll * ntt_a[i] * ntt_b[i] % mod * jc[i] % mod;
    for(int i=degA+degB+1;i<lim;i++) ntt_a[i] = 0;
    for(int i=0;i<=degA+degB;i++) ntt_c[i] = (i&1) ? mod - ijc[i] : ijc[i];
    for(int i=degA+degB+1;i<lim;i++) ntt_c[i] = 0;
    dft(ntt_a, lim); dft(ntt_c, lim);
    for(int i=0;i<lim;i++) ntt_a[i] = 1ll * ntt_a[i] * ntt_c[i] % mod;
    idft(ntt_a, lim);
    for(int i=0;i<=degA+degB;i++) C[i] = ntt_a[i];
}

int n, m;
int f[N], g[N], ans[N];

int main() {
    initeps();
    jc[0] = jc[1] = ijc[0] = ijc[1] = 1;
    for(int i=2;i<N;i++) jc[i] = 1ll * jc[i-1] * i % mod;
    ijc[N-1] = inv(jc[N-1]);
    for(int i=N-2;i;i--) ijc[i] = 1ll * ijc[i+1] * (i+1) % mod;

    read(n); read(m);
    for(int i=0;i<=n;i++) read(f[i]);
    for(int i=0;i<=m;i++) read(g[i]);
    FFPmul(ans, f, g, n, m);
    for(int i=0;i<=n+m;i++) { write(ans[i]); putchar(" \n"[i==n+m]);}
    return 0;
}