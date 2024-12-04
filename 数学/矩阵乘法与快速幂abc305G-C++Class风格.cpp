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

template <typename T>
void _debug(const char* format, T t) {
    cerr << format << '=' << t << endl;
}

template <class First, class... Rest>
void _debug(const char* format, First first, Rest... rest) {
    while (*format != ',') cerr << *format++;
    cerr << '=' << first << ',';
    _debug(format + 1, rest...);
}

template <typename T>
ostream& operator<<(ostream& os, const vector<T>& V) {
    os << "[ ";
    for (const auto& vv : V) os << vv << ", ";
    os << ']';
    return os;
}
#ifdef LOCAL
    #define dbg(...) _debug(#__VA_ARGS__, __VA_ARGS__)
#else
    #define dbg(...) 
#endif

template<typename Tp> IL void read(Tp &x) {
    x=0; int f=1; char ch=getchar();
    while(!isdigit(ch)) {if(ch == '-') f=-1; ch=getchar();}
    while(isdigit(ch)) { x=x*10+ch-'0'; ch=getchar();}
    x *= f;
}
template<typename First, typename... Rest> IL void read(First &first, Rest&... rest) {
    read(first); read(rest...);
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
template<typename First, typename... Rest> IL void write(const First& first, const Rest&... rest) {
    write(first); putchar(32); write(rest...);
}

template<class T> class Matrix {
public:
    int n, m;
    vector<vector<T> > v;
    Matrix(): Matrix(0, 0) {}
    explicit Matrix(int n, int m): Matrix(vector<vector<T> >(n, vector<T>(m))) {}
    explicit Matrix(const vector<vector<T> >& rhs) {
        n = rhs.size();
        m = rhs[0].size();
        for (int i = 0; i < n; i++) assert((int)rhs[i].size() == m);
        v = rhs;
    }
    Matrix<T> operator * (const Matrix<T>& rhs) const {
        assert(m == rhs.n);
        Matrix<T> ret = Matrix<T>(n, rhs.m);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                ret.v[i][j] = 0;
                for (int k = 0; k < rhs.m; k++) {
                    ret.v[i][j] += v[i][k] * rhs.v[k][j];
                }
            }
        }
        return ret;
    }
    void print() {
        cerr << "Matrix n = " << n << ", m = " << m << "\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                cerr << v[i][j].val() << " \n"[j == m - 1];
            }
        }
    }
};
template<class T, class Tp> Matrix<T> matksm(Matrix<T> a, Tp b) {
    int n = a.n, m = a.m;
    assert(n == m);
    Matrix<T> ret = Matrix<T>(n, n);
    for (int i = 0; i < n; i++) ret.v[i][i] = 1;
    while (b > 0) {
        if (b % 2 == 1) ret = ret * a;
        a = a * a;
        b >>= 1;
    }
    return ret;
}
#include <atcoder/modint>
using mint = atcoder::modint998244353;

const int N = 128;

void solve() {
    ios::sync_with_stdio(0); cin.tie(0);
    ll n; int m; cin >> n >> m;
    vector<string> ban(m);
    for (int i = 0; i < m; i++) cin >> ban[i];
    sort(ban.begin(), ban.end());
    int totid = 0;
    map<string, int> id;
    vector<string> sid(N);
    queue<int> q;
    q.push(0);
    id[""] = totid++;
    sid[0] = "";
    Matrix<mint> A(N, N);
    int cnt = 0;
    // cout << "begin bfs" << "\n";
    while (!q.empty()) {
        int u = q.front(); q.pop();
        string now = sid[u];
        if (sid[u].length() == 6) now = now.substr(1, now.length() - 1);
        for (char ch = 'a'; ch <= 'b'; ch++) {
            string nxt = now + ch;
            bool isban = false;
            for (int l = 1; l <= (int)nxt.length(); l++) {
                cnt++;
                if (*lower_bound(ban.begin(), ban.end(), nxt.substr(nxt.length() - l, l)) == nxt.substr(nxt.length() - l, l)) {
                    isban = true;
                    break;
                }
            }
            if (isban) continue;
            if (id.find(nxt) == id.end()) id[nxt] = totid++;
            sid[id[nxt]] = nxt;
            A.v[u][id[nxt]] = 1;
            if (sid[u].length() != 6) q.push(id[nxt]);
        }
    }
    // dbg(cnt);
    Matrix<mint> ans(1, N);
    ans.v[0][0] = 1;
    // A.print();
    A = matksm(A, n);
    // A.print();
    ans = ans * A;
    // ans.print();
    mint out = 0;
    // for (int i = 0; i < N; i++) if (ans.v[0][i] != 0) {
    //     cout << "sid, " << i << ", " << sid[i] << "\n";
    // }
    for (int i = 0; i < N; i++) {out += ans.v[0][i];}
    cout << out.val() << "\n";
}

int main() {
#ifdef LOCAL
    freopen("test.in", "r", stdin);
    // freopen("test.out", "w", stdout);
#endif
    int T = 1;
    // read(T);
    while(T--) solve();
    return 0;
}