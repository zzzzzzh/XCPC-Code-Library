#include <bits/stdc++.h>

typedef long long ll;
const int N = 40000010; // $N$ 要比 $\sqrt{n}$ 后的一个质数大
const int M = N << 1;

int min[N];
std::vector<int> pri;
std::vector<ll> pre; // 所求函数 1 及质数处的前缀和
ll f[M];
ll n;

void init() {
    pre.push_back(1);
    for (int i = 2; i < N; ++i) {
        if (!min[i]) {
            min[i] = i;
            pri.push_back(i);
            pre.push_back(pre.back() + 1); // 1 应修改为 f(p)
        }
        for (int j = 0, sz = pri.size(); j < sz && i * pri[j] < N; ++j) {
            min[i * pri[j]] = pri[j];
            if (i % pri[j] == 0) break;
        }
    }
}

inline int getsit(ll x) {
    return x < N ? x : M - n / x;
}

ll calcf(int p, int j){
    ll sum = 0, now = 1;
    for (int i = 0; i <= j; ++ i){
        sum += now;
        if (i & 1){
            now *= p;
        }
    }
    return sum;
}

void calcpri() {
    std::vector<ll> vec;
    std::vector<int> id;
    for (ll i = 1, j; i <= n; i = j + 1) {
        ll x = n / i;
        int sit = getsit(x);
        j = n / x;
        // 质数处的函数值要求是个多项式，这里对每一个幂次需要分别计算
        // 计算将该幂次推广到 N^* 上的前缀和
        f[sit] = sum(x);
        vec.push_back(x);
        id.push_back(sit);
    }
    for (int k = 0, szk = pri.size(); k < szk; ++k) {
        int p = pri[k];
        if (1ll * p * p > n) break;
        for (int i = 0, szi = vec.size(); i < szi; ++i) {
            ll x = vec[i];
            if (x < 1ll * p * p) break;
            int sit = getsit(x / p);
            // 这里记得乘上当前计算的质数幂次
            f[id[i]] -= (f[sit] - pre[k]) * power(p, e);
        }
    }
}

ll calcsum(ll n, int sit) { // main 中调用 1 + calcsum(n, 0)
    if (pri[sit] > n) return 0;
    int id = getsit(n);
    ll ret = f[id] - pre[sit]; // >=p[sit] 的质数处和
    for (int i = sit, sz = pri.size(); i < sz; ++i) { // 枚举最小质因子
        int p = pri[i];
        ll now1 = p, now2 = 1ll * p * p;
        if (now2 > n) break;
        for (int j = 1; now1 <= n / p; ++j, now1 = now2, now2 *= p){
            ret += calcf(p, j) * calcsum(n / now1, i + 1) + calcf(p, j + 1);
        }
    }
    return ret;
}
