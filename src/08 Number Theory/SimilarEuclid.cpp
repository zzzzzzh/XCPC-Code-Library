<TeX>类欧模板。求解 $\sum_{x=0}^{n}x^{t_{1}}\lfloor\frac{ax+b}{c}\rfloor^{t_{2}}$。时间复杂度 $\mathcal{O}((t_{1}+t_{2})^{4}\log n)$。如果 $t_{1}=0$ 且 $t_{2}=1$，注意有简化版本可用。</TeX>
#include<bits/stdc++.h>

const int N = 60;
const int M = 20;
const int K = M + 10;
const int moder = (int) 1e9 + 7;
typedef long long ll;

int dp[2][N][M][M];
int comb[K][K];
int g[M][M], h[M][K];
int bern[M];
int inv[K];

void init(){
    for (int i = 0; i < K; ++ i){
        comb[i][0] = 1;
        for (int j = 1; j <= i; ++ j){
            comb[i][j] = comb[i - 1][j - 1] + comb[i - 1][j];
            comb[i][j] -= comb[i][j] >= moder ? moder : 0;
        }
    }
    inv[0] = 0, inv[1] = 1;
    for (int i = 2; i < K; ++ i){
        inv[i] = (moder - 1ll * (moder / i) * inv[moder % i] % moder) % moder;
    }
    bern[0] = 1;
    for (int i = 1; i < M; ++ i){
        bern[i] = 1;
        for (int j = 0; j <= i - 1; ++ j){
            bern[i] = (bern[i] - 1ll * comb[i][j] * bern[j] % moder * inv[i - j + 1]) % moder;
        }
        bern[i] += bern[i] < 0 ? moder : 0;
    }
    for (int i = 1; i < M; ++ i){
        for (int j = 0; j <= i - 1; ++ j){
            g[i][j] = comb[i][j];
        }
    }
    for (int i = 0; i < M; ++ i){
        for (int j = 0; j <= i; ++ j){
            h[i][i + 1 - j] = 1ll * comb[i + 1][j] * bern[j] % moder * inv[i + 1] % moder;
        }
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

int hvalue(int sit, int x){
    int ret = 0, now = 1;
    for (int i = 0; i <= sit + 1; ++ i){
        ret = (ret + 1ll * h[sit][i] * now) % moder;
        now = 1ll * now * x % moder;
    }
    return ret;
}

int calc(int a, int b, int c, int n, int t1, int t2, int type, int sit){
    int &calcdp = dp[type][sit][t1][t2];
    if (calcdp != -1){
        return calcdp;
    }
    if (!t2){
        calcdp = hvalue(t1, n) + (t1 == 0);
        calcdp -= calcdp >= moder ? moder : 0;
        return calcdp;
    }
    ll m = (1ll * a * n + b) / c;
    if (!m){
        return 0;
    }
    if (!a){
        return calcdp = 1ll * powermod(b / c, t2) * (hvalue(t1, n) + (t1 == 0)) % moder;
    }
    if (type == 0){
        if (a >= c || b >= c){
            int x1 = a / c, x2 = b / c, ret = 0, now1 = 1;
            for (int u2 = 0; u2 <= t2; ++ u2){
                int now2 = 1;
                for (int u3 = 0; u2 + u3 <= t2; ++ u3){
                    int u1 = t2 - u2 - u3;
                    int coe = 1ll * comb[t2][u1] * comb[t2 - u1][u2] % moder;
                    coe = 1ll * coe * now1 % moder * now2 % moder;
                    ret = (ret + 1ll * coe * calc(a % c, b % c, c, n, t1 + u2, u1, type ^ 1, sit)) % moder;
                    now2 = 1ll * now2 * x2 % moder;
                }
                now1 = 1ll * now1 * x1 % moder;
            }
            return calcdp = ret;
        }
        return calcdp = calc(a, b, c, n, t1, t2, type ^ 1, sit);
    }
    int ret = 1ll * powermod(m, t2) * hvalue(t1, n) % moder;
    for (int u = 0; u <= t2 - 1; ++ u){
        for (int v = 0; v <= t1 + 1; ++ v){
            ret = (ret - 1ll * g[t2][u] * h[t1][v] % moder * calc(c, c - b - 1, a, m - 1, u, v, type ^ 1, sit + 1)) % moder;
        }
    }
    ret += ret < 0 ? moder : 0;
    return calcdp = ret;
}

int calc(int a, int b, int c, int n){ //special case for t1 = 0, t2 = 1
    ll m = (1ll * a * n + b) / c;
    if (!m){
        return 0;
    }
    if (!a){
        return 1ll * (b / c) * (n + 1) % moder;
    }
    if (a >= c || b >= c){
        int ret = (1ll * n * (n + 1) >> 1) % moder * (a / c) % moder;
        ret = (ret + 1ll * (n + 1) * (b / c)) % moder;
        ret += calc(a % c, b % c, c, n);
        ret -= ret >= moder ? moder : 0;
        return ret;
    }
    int ret = (1ll * n * m - calc(c, c - b - 1, a, m - 1)) % moder;
    ret += ret < 0 ? moder : 0;
    return ret;
}

int solve(int a, int b, int c, int n, int t1, int t2){
    memset(dp, -1, sizeof(dp));
    int ret = calc(a, b, c, n, t1, t2, 0, 0);
    if (t1 == 0 && t2 == 1){
        assert(ret = calc(a, b, c, n));
    }
    return ret;
}

int main(){
}
