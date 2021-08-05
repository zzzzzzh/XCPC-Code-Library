<TeX>大阶乘模质数的幂 $p^{e}$。时间复杂度 $\mathcal{O}(pe+e\log_{p}n)$。</TeX>
#include<bits/stdc++.h>

const int N = 1010;
typedef std::pair <int, int> pii;

int moder;
int s[N][N];
int inv[N], invcnt[N];
int power[N];
int poly[N];
int inter[N];

int eucinv(int a, int moder){
    int b = moder, s = 1, t = 0;
    while (b){
        int tmp = a, q = a / b;
        a = b, b = tmp % a;
        tmp = s;
        s = t;
        t = tmp - s * q;
    }
    if (a > 1) return -1;
    return s < 0 ? s + moder : s;
}

void initpower(int p, int e){
    power[0] = 1;
    for (int i = 1; i <= e; ++ i){
        power[i] = power[i - 1] * p;
    }
}

void inits(int p, int e){
    s[0][0] = 1;
    for (int i = 1; i <= p; ++ i){
        for (int j = 1, sz = std::min(p, e); j <= sz; ++ j){
            s[i][j] = j > i ? 0 : (1ll * (i - 1) * s[i - 1][j] + s[i - 1][j - 1]) % moder;
        }
    }
}

void initinv(int p, int e){
    inv[0] = 1;
    for (int i = 1, sz = std::max(2 * e - 2, p - 1); i <= sz; ++ i){
        int x = i;
        invcnt[i] = 0;
        while (x % p == 0){
            ++ invcnt[i];
            x /= p;
        }
        invcnt[i] += invcnt[i - 1];
        inv[i] = 1ll * inv[i - 1] * eucinv(x, moder) % moder;
    }
}

int calcinter(int u, int v, int p, int e){
    int ans = 0, now = 1, mult = 1ll * u * p % moder;
    for (int i = 0, sz = std::min(v, e - 1); i <= sz; ++ i){
        ans = (ans + 1ll * now * s[v + 1][i + 1]) % moder;
        now = 1ll * now * mult % moder;
    }
    return ans;
}

void initpoly(int p, int e){
    poly[0] = 1;
    for (int i = 1; i <= 2 * e - 2; ++ i){
        poly[i] = 1ll * poly[i - 1] * calcinter(i - 1, p - 1, p, e) % moder * inv[p - 1] % moder;
    }
    for (int i = 0; i <= 2 * e - 2; ++ i){
        inter[i] = 1ll * poly[i] * inv[i] % moder * inv[2 * e - 2 - i] % moder;
    }
}

int powermod(int a, int exp){
    int ret = 1;
    for ( ; exp > 0; exp >>= 1){
        if (exp & 1){
            ret = 1ll * ret * a % moder;
        }
        a = 1ll * a * a % moder;
    }
    return ret;
}

pii solve(int n, int p, int e){
    initpower(p, e);
    moder = power[e];
    inits(p, e);
    initinv(p, e);
    initpoly(p, e);
    int max = e + invcnt[2 * e - 2];
    int ans = 1, cnt = 0;
    while (n){
        int u = n / p, v = n % p;
        n = u;
        cnt += u;
        ans = 1ll * ans * calcinter(u, v, p, e) % moder;
        int sz = 2 * e - 2;
        if (u <= sz){
            ans = 1ll * ans * poly[u] % moder;
            continue;
        }
        std::vector <pii> pre(sz + 1), suf(sz + 1);
        pre[0] = suf[sz] = {1, 0};
        for (int i = 1; i <= sz; ++ i){
            int x = u - i + 1;
            while (x % p == 0 && pre[i].second < max){
                ++ pre[i].second;
                x /= p;
            }
            pre[i].second += pre[i - 1].second;
            pre[i].first = 1ll * pre[i - 1].first * x % moder;
        }
        for (int i = sz - 1; i >= 0; -- i){
            int x = u - i - 1;
            while (x % p == 0 && suf[i].second < max){
                ++ suf[i].second;
                x /= p;
            }
            suf[i].second += suf[i + 1].second;
            suf[i].first = 1ll * suf[i + 1].first * x % moder;
        }
        int sum = 0;
        for (int i = 0; i <= sz; ++ i){
            int cnt = pre[i].second + suf[i].second - invcnt[i] - invcnt[sz - i];
            if (cnt >= e) continue;
            int tmp = power[cnt];
            //<==>(2 * e - 2 - i & 1 ? -1ll : 1ll)
            sum = (sum + (i & 1 ? -1ll : 1ll) * tmp * pre[i].first % moder * suf[i].first % moder * inter[i]) % moder;
        }
        sum += sum < 0 ? moder : 0;
        ans = 1ll * ans * sum % moder;
    }
    ans = 1ll * ans * powermod(s[p][1], cnt) % moder;
    return {ans, cnt};
}

int main(){
    int n, p, e;
    scanf("%d%d%d", &n, &p, &e);
    pii ans = solve(n, p, e);
    ans.first = 1ll * ans.first * (ans.second >= e ? 0 : power[ans.second]) % moder;
    printf("%d\n", ans.first);
    return 0;
}
