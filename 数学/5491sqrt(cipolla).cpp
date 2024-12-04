#include <bits/stdc++.h>
using namespace std;

#define IL inline
#define dbg1(x) cout << #x << " = " << x << ", "
#define dbg2(x) cout << #x << " = " << x << endl

template<typename Tp> IL void read(Tp& x) {
    x=0; int f=1; char ch=getchar();
    while(!isdigit(ch)) {if(ch =='-') f=-1; ch=getchar();}
    while(isdigit(ch)) {x = x*10+ch-'0'; ch=getchar();}
    x *= f;
}
int buf[22];
template<typename Tp> IL void write(Tp x) {
    int p = 0;
    if(x < 0) { putchar('-'); x=-x;}
    if(x == 0) {buf[++p] = 0;}
    else while(x) {
        buf[++p] = x % 10;
        x /= 10;
    }
    for(int i=p;i;i--) putchar(buf[i]+'0');
}

IL int ksm(int a, int b, int p) {
    int ret = 1;
    while(b) {
        if(b&1) ret = 1ll * ret * a % p;
        a = 1ll * a * a % p;
        b >>= 1;
    }
    return ret;
}

int cp2i2;
struct cp2 {
    int x, y;
    cp2(int x=0, int y=0):x(x), y(y) {}
};
IL cp2 mul(cp2 a, cp2 b, int p) {
    cp2 ret(0,0);
    ret.x = ((1ll * a.x * b.x % p + 1ll * a.y * b.y % p * cp2i2 % p) % p + p) % p;;
    ret.y = (1ll * a.x * b.y % p + 1ll * a.y * b.x % p) % p;
    return ret;
}
IL int cp2ksm(cp2 a, int b, int p) {
    cp2 ret(1,0);
    while(b) {
        if(b&1) ret = mul(ret, a, p);
        a = mul(a, a, p);
        b >>= 1;
    }
    return ret.x;
}

IL int cipolla(int n, int p, int &x1, int &x2) {
    n %= p;
    if(p == 2) return n;
    if(n == 0) {x1 = x2 = 0; return 0;}
    if(ksm(n, (p-1) / 2, p) == p - 1) return -1;
    int a;
    while(1) {
        a = ((rand() << 15) | rand()) % p;
        cp2i2 = (1ll * a * a % p - n + p) % p;
        if(ksm(cp2i2, (p-1) >> 1, p) == p - 1) break;
    }
    cp2 x(a, 1);
    x1 = cp2ksm(x, (p+1) >> 1, p);
    x2 = p - x1;
    if(x1 > x2) swap(x1, x2);
    return 0;
}

int main() {
    int T; read(T);
    int x1, x2;
    while(T--) {
        int n, p; read(n); read(p);
        int x1, x2;
        if(cipolla(n, p, x1, x2) == -1) {puts("Hola!"); continue;}
        if(x1 == x2) {write(x1); putchar(10);}
        else {write(x1); putchar(32); write(x2); putchar(10);}
    }
    return 0;
}