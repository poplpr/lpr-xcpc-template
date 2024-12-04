#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef double db;
typedef long double ld;

#define IL inline
#define fi first
#define se second
#define mk make_pair
#define pb push_back
#define SZ(x) (int)(x).size()
#define ALL(x) (x).begin(), (x).end()
#define dbg1(x) cout << #x << " = " << x << ", "
#define dbg2(x) cout << #x << " = " << x << endl

template<typename Tp> IL void read(Tp &x) {
    x=0; int f=1; char ch=getchar();
    while(!isdigit(ch)) {if(ch == '-') f=-1; ch=getchar();}
    while(isdigit(ch)) { x=x*10+ch-'0'; ch=getchar();}
    x *= f;
}
int buf[42];
template<typename Tp> IL void write(Tp x) {
    int p = 0;
    if(x < 0) { putchar('-'); x=-x;}
    if(x == 0) { putchar('0'); return;}
    while(x) {
        buf[++p] = x % 10;
        x /= 10;
    }
    for(int i=p;i;i--) putchar('0' + buf[i]);
}

const int N = 50000 + 5;

struct ASK {
    int l, r, id, pos;
    ASK(int l=0, int r=0, int id=0, int pos=0):l(l),r(r),id(id),pos(pos) {}
};

int n, m, K, blocksz = 0;
ll cur=0;
int a[N], cnt[N];
ll ans[N];

ASK ask[N];

IL void add(int x) {
    cur += cnt[a[x]] * 2 + 1;
    cnt[a[x]]++;
}
IL void del(int x) {
    cnt[a[x]]--;
    cur -= cnt[a[x]] * 2 + 1;
}

int main() {
#ifdef LOCAL
    freopen("test.in", "r", stdin);
    // freopen("test.out", "w", stdout);
#endif
    read(n); read(m); read(K);
    for(int i=1;i<=n;i++) read(a[i]);
    blocksz = ceil(1.0 * n / sqrt(m));
    for(int i=1;i<=m;i++) {
        int l, r; read(l); read(r);
        ask[i] = ASK(l, r, i, (l - 1) / blocksz + 1);
    }
    sort(ask+1, ask+1+m, [](ASK x, ASK y) { 
        return x.pos ^ y.pos ? x.pos < y.pos : x.pos & 1 ? x.r < y.r : x.r > y.r;
    });
    for(int i=1, l=1, r=0;i<=m;i++) {
        while(l > ask[i].l) add(--l);
        while(r < ask[i].r) add(++r);
        while(l < ask[i].l) del(l++);
        while(r > ask[i].r) del(r--);
        ans[ask[i].id] = cur;
    }
    for(int i=1;i<=m;i++) {
        write(ans[i]); putchar(10);
    }
    return 0;
}