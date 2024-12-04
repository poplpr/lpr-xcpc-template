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

int ntt_a[N], ntt_b[N];

IL void CDQ_NTT(int *B, int *A, int L, int R) {
    if(L == R) { if(L == 0) B[0] = 1; return;}
    int M = L+R >> 1;
    CDQ_NTT(B, A, L, M);
    int lim = 1, lglim = 0;
    while(lim <= (R-L+1)) {lim <<= 1; lglim++;}
    for(int i=0;i<lim;i++) rev[i] = (rev[i>>1] >> 1) | ((i&1) << (lglim-1));
    for(int i=0;i+L<=M;i++) ntt_a[i] = B[i+L];
    for(int i=M-L+1;i<lim;i++) ntt_a[i] = 0;
    for(int i=0;i+L<=R;i++) ntt_b[i] = A[i];
    for(int i=R-L+1;i<lim;i++) ntt_b[i] = 0;
    dft(ntt_a, lim); dft(ntt_b, lim);
    for(int i=0;i<lim;i++) ntt_a[i] = 1ll * ntt_a[i] * ntt_b[i] % mod;
    idft(ntt_a, lim);
    for(int i=M+1;i<=R;i++) B[i] = (B[i] + ntt_a[i-L]) % mod;
    CDQ_NTT(B, A, M+1, R);
}

int n;
int f[N], ans[N];

int main() {
    initeps();
    read(n); n--;
    for(int i=1;i<=n;i++) read(f[i]);
    CDQ_NTT(ans, f, 0, n);
    for(int i=0;i<=n;i++) {write(ans[i]); putchar(" \n"[i==n]);}
    return 0;
}