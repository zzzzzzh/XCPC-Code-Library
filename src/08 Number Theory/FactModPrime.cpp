<TeX>大阶乘模质数。时间复杂度 $\mathcal{O}(\sqrt{n}\log n)$。</TeX>
#include<bits/stdc++.h>

const int N = 500010;
const int MAX = 16;
typedef long long ll;

int w[2][1 << MAX];
int moder;
int root;
int invroot;
int fac[N], invf[N], inv[N];

int powermod(int a, int exp){
    int ret = 1;
    for ( ; exp > 0; exp >>= 1){
        if (exp & 1){
            ret = 1ll * a * ret % moder;
        }
        a = 1ll * a * a % moder;
    }
    return ret;
}

void findroot(){
    int x = moder - 1;
    std::vector <int> prime;
    for (int i = 2; i * i <= x; ++i){
        if (x % i == 0){
            prime.push_back(i);
            while (x % i == 0){
                x /= i;
            }
        }
    }
    if (x > 1){
        prime.push_back(x);
    }
    for (int i = 2; ; ++i){
        bool flag = true;
        for (auto u : prime){
            if (powermod(i, (moder - 1) / u) == 1){
                flag = false;
                break;
            }
        }
        if (flag){
            root = i;
            invroot = powermod(i, moder - 2);
            return;
        }
    }
}

void init(){
    findroot();
    w[0][0] = w[1][0] = 1;
    w[0][1] = powermod(root, (moder - 1) >> MAX);
    for (int i = 2; i < (1 << MAX); ++i){
        w[0][i] = 1ll * w[0][i - 1] * w[0][1] % moder;
    }
    for (int i = 1; i < (1 << MAX); ++i){
        w[1][(1 << MAX) - i] = w[0][i];
    }
    fac[0] = invf[0] = 1;
    for (int i = 1; i < N; ++i){
        fac[i] = 1ll * fac[i - 1] * i % moder;
        inv[i] = i == 1 ? 1 : (moder - 1ll * (moder / i) * inv[moder % i] % moder) % moder;
        invf[i] = 1ll * invf[i - 1] * inv[i] % moder;
    }
}

void NTT(std::vector <int> &a, int len, int type){
    for(int i = 1, j = 0; i < len - 1; ++i){
        for(int s = len; j ^= (s >>= 1), ~j & s; )
            ;
        if(i < j){
            std::swap(a[i], a[j]);
        }
    }
    for (int i = 2, d = 1; i <= len; i <<= 1, ++d){
        for (int j = 0; j < len; j += i){
            auto u = a.begin() + j;
            for (int k = 0; k < i / 2; ++k){
                int t = 1ll * w[type][k << (MAX - d)] * u[k + i / 2] % moder;
                u[k + i / 2] = u[k] - t + (u[k] - t < 0 ? moder : 0);
                ll x = (ll) u[k] + t;
                x -= x >= moder ? moder : 0;
                u[k] = x;
            }
        }
    }
    if (type == 0) return;
    for (int i = 0; i < len; ++i){
        a[i] = 1ll * a[i] * inv[len] % moder;
    }
}

struct poly{
    std::vector<int> a;
    int len;
    
    poly() : len(-1) {}
    poly(int len) : len(len) { a.resize(len + 1); }
    poly(const poly &p, int len) : len(len) { 
        a.resize(len + 1); 
        for (int i = 0; i <= len; ++i) {
            a[i] = i > p.len ? 0 : p.a[i];
        }
    }
    
    int &operator [] (int n) { assert(n >= 0 && n <= len); return a[n]; }

    poly operator * (const poly &p) const {
        if (!~len || !~p.len) return poly(-1);
        int n = len + p.len;
        int lenret = 1;
        for ( ; lenret <= n; lenret <<= 1)
            ;
        poly ret(*this, lenret);
        std::vector<int> aux(lenret);
        std::copy(p.a.begin(), p.a.begin() + p.len + 1, aux.begin());
        NTT(ret.a, lenret, 0);
        NTT(aux, lenret, 0);
        for (int i = 0; i < lenret; ++i) {
            ret.a[i] = 1ll * ret.a[i] * aux[i] % moder;
        }
        NTT(ret.a, lenret, 1);
        ret.len = n;
        return ret;
    }
};

poly shift(poly p, int dis){
    int d = p.len;
    //这部分在moder很小的时候不正确
    poly p1(d + 1), p2(2 * d + 1);
    for (int i = 0; i <= d; ++i){
        p1[i] = ((d - i) & 1 ? -1ll : 1ll) * p[i] * invf[i] % moder * invf[d - i] % moder;
        p1[i] += p1[i] < 0 ? moder : 0;
    }
    for (int i = 0; i <= d << 1; ++i){
        p2[i] = powermod(i - d + dis, moder - 2);
    }
    p1 = p1 * p2;
    poly ret(d);
    int coe = 1;
    for (int i = 0; i <= d; ++i){
        coe = 1ll * coe * (dis - i) % moder;
    }
    for (int i = d; i <= d << 1; ++i){
        ret.a[i - d] = 1ll * p1[i] * coe % moder;
        coe = 1ll * coe * (dis + i - d + 1) % moder * powermod(dis - 2 * d + i, moder - 2) % moder;
    }
    return ret;
}

poly facmod(int d, int v){
    if (d == 1){
        poly p(1);
        p[0] = 1;
        p[1] = v + 1;
        return p;
    }
    int x = d >> 1;
    poly p = facmod(x, v);
    int y = 1ll * inv[v] * x % moder;
    poly p1 = shift(p, x + 1);
    poly p2 = shift(p, y);
    poly p3 = shift(p, y + x + 1);
    poly q(x << 1);
    for (int i = 0; i <= x; ++i){
        q[i] = 1ll * p[i] * p2[i] % moder;
        if (i < x) q[i + x + 1] = 1ll * p1[i] * p3[i] % moder;
    }
    if (!(d & 1)) return q;
    p = poly(d);
    p[d] = 1;
    for (int i = 0; i < d; ++i){
        p[i] = 1ll * q[i] * (i * v + d) % moder;
        p[d] = 1ll * p[d] * (d * v + i + 1) % moder;
    }
    return p;
}

int solve(int n){
    if (n < N){
        return fac[n];
    }
    int v = std::sqrt(n);
    poly p = facmod(v, v);
    int ans = 1;
    for (int i = 0; i < v; ++i){
        ans = 1ll * ans * p[i] % moder;
    }
    for (int i = v * v + 1; i <= n; ++i){
        ans = 1ll * ans * i % moder;
    }
    return ans;
}

int main(){
    int n;
    scanf("%d%d", &n, &moder);
    init();
    if (n >= moder){
        puts("0");
        return 0;
    }
    int ans;
    if (n > moder >> 1){
        ans = powermod(solve(moder - n - 1), moder - 2);
        ans *= (moder - n) & 1 ? -1 : 1;
        ans += ans < 0 ? moder : 0;
    }
    else{
        ans = solve(n);
    }
    if (n & 1){
        ans = 1ll * ((moder + 1) >> 1) * ans % moder;
    }
    printf("%d\n", ans);
    return 0;
}
