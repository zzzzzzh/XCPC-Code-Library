<TeX>序列求和V5。对 $p^{e}$ 取模时，时间复杂度 $\mathcal{O}(k+e+\log_{p}x)$。</TeX>
#include<bits/stdc++.h>

int inv(int a, int moder){
    int b = moder, s = 1, t = 0;
    while (b){
        int tmp = a, q = a / b;
        a = b, b = tmp % a;
        tmp = s;
        s = t;
        t = tmp - s * q;
    }
    assert(a == 1);
    return s < 0 ? s + moder : s;
}

typedef std::pair <int, int> pii;
typedef long long ll;

pii crt(pii p1, pii p2){
    int moder = p1.second * p2.second;
    int remain = (1ll * p1.first * inv(p2.second, p1.second) % moder * p2.second + 1ll * p2.first * inv(p1.second, p2.second) % moder * p1.second) % moder;
    return {remain, moder};
}

const int N = 2010;

int moder;
int p, e;
int a[N];
int power[N];
int fac[N], invf[N], cnt[N];

void initpower(){
    power[0] = 1;
    for (int i = 1; i <= e; ++ i){
        power[i] = power[i - 1] * p;
    }
    moder = power[e];
}

void initfac(){
    fac[0] = invf[0] = 1;
    for (int i = 1; i < N; ++ i){
        cnt[i] = 0;
        int x = i;
        while (x % p == 0){
            x /= p;
            ++ cnt[i];
        }
        cnt[i] += cnt[i - 1];
        fac[i] = 1ll * fac[i - 1] * x % moder;
        invf[i] = 1ll * invf[i - 1] * inv(x, moder) % moder;
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

int comb(int n, int m){
    int ccnt = cnt[n] - cnt[m] - cnt[n - m];
    if (ccnt >= e) return 0;
    return 1ll * fac[n] * invf[m] % moder * invf[n - m] % moder * power[ccnt] % moder;
}

int interpolation(std::vector <int> vec, int x){
    int n = vec.size() - 1;
    if (x >= 0 && x <= n) return vec[x];
    std::vector <pii> pre(n + 1), suf(n + 1);
    pre[0] = suf[n] = {1, 0};
    for (int i = 1; i <= n; ++ i){
        int y = x - i + 1;
        while (y % p == 0){
            ++ pre[i].second;
            y /= p;
        }
        pre[i].second += pre[i - 1].second;
        pre[i].first = 1ll * pre[i - 1].first * y % moder;
    }
    for (int i = n - 1; i >= 0; -- i){
        int y = x - i - 1;
        while (y % p == 0){
            ++ suf[i].second;
            y /= p;
        }
        suf[i].second += suf[i + 1].second;
        suf[i].first = 1ll * suf[i + 1].first * y % moder;
    }
    int ans = 0;
    for (int i = 0; i <= n; ++ i){
        int ccnt = pre[i].second + suf[i].second - cnt[i] - cnt[n - i];
        if (ccnt >= e) continue;
        ans = (ans + ((n - i) & 1 ? -1ll : 1ll) * vec[i] * power[ccnt] % moder * pre[i].first % moder * suf[i].first % moder * invf[i] % moder * invf[n - i]) % moder;
    }
    ans += ans < 0 ? moder : 0;
    return ans;
}

int calc(std::vector <int> vec, int x, int b){
    int n = vec.size() - 1;
    std::vector <int> vec1;
    if (b % p == 1){
        int now = b, sum = 0;
        vec1 = {0};
        for (int i = 1; i <= n + e; ++ i){
            if (i <= n){
                sum = (sum + 1ll * vec[i] * now) % moder;
            }
            else{
                sum = (sum + 1ll * interpolation(vec, i) * now) % moder;
            }
            vec1.push_back(sum);
            now = 1ll * now * b % moder;
        }
        return interpolation(vec1, x);
    }
    int invb = inv(b, moder), now = b;
    vec.push_back(interpolation(vec, n + 1));
    vec[0] = 0;
    for (int i = 1; i <= n + 1; ++ i){
        vec[i] = (vec[i - 1] + 1ll * vec[i] * now) % moder;
        now = 1ll * now * b % moder;
    }
    int sum = 0;
    now = invb;
    for (int i = 1; i <= n + 1; ++ i){
        sum = (sum + (i & 1 ? 1ll : -1ll) * comb(n + 1, i) * vec[i] % moder * now) % moder;
        now = 1ll * now * invb % moder;
    }
    sum += sum < 0 ? moder : 0;
    int x0 = 1ll * sum * inv(powermod(1ll * (b - 1) * invb % moder, n + 1), moder) % moder;
    vec1.push_back(x0);
    now = invb;
    for (int i = 1; i <= n; ++ i){
        vec1.push_back(((ll) x0 + vec[i]) * now % moder);
        now = 1ll * now * invb % moder;
    }
    int ans = (1ll * powermod(b, x) * interpolation(vec1, x) - x0) % moder;
    ans += ans < 0 ? moder : 0;
    return ans;
}

int solve(int n, int l, int r, int b){
    initpower();
    initfac();
    std::vector <int> vec;
    for (int i = 0; i <= n; ++ i){
        vec.push_back(a[i]);
    } 
    if (b % p == 0){
        int ans = 0, now = b % moder;
        for (int i = r; i >= l && now; -- i){
            ans = (ans + 1ll * interpolation(vec, i) * now) % moder;
            now = 1ll * now * b % moder;
        }
        return ans;
    }
    int invb = inv(b, moder);
    if (0 <= l){
        int ans = calc(vec, r, invb) - calc(vec, std::max(0, l - 1), invb);
        ans += ans < 0 ? moder : 0;
        ans = 1ll * ans * powermod(b, r + 1) % moder;
        if (l == 0){
            ans = (ans + 1ll * a[0] * powermod(b, r + 1)) % moder;
        }
        return ans;
    }
    if (r <= n){
        std::reverse(vec.begin(), vec.end());
        int ans = calc(vec, n - l, b) - calc(vec, std::max(n - r - 1, 0), b);
        ans += ans < 0 ? moder : 0;
        ans = 1ll * ans * powermod(invb, n - r - 1) % moder;
        if (r == n){
            ans = (1ll * ans * b + 1ll * a[n] * b) % moder;
        }
        return ans;
    }
    int ans = 1ll * calc(vec, r, invb) * powermod(b, r + 1) % moder;
    std::reverse(vec.begin(), vec.end());
    ans = (ans + 1ll * calc(vec, n - l, b) * powermod(b, r - n + 1)) % moder;
    int now = powermod(b, r);
    for (int i = 1; i < n; ++ i){
        ans = (ans - 1ll * now * a[i]) % moder;
        now = 1ll * now * invb % moder;
    }
    ans += ans < 0 ? moder : 0;
    return ans;
}

int main(){
    int n, l, r, b, m;
    scanf("%d%d%d%d%d", &n, &l, &r, &b, &m);
    std::vector <pii> vec;
    for (int i = 2; i * i <= m; ++ i){
        if (m % i == 0){
            int cnt = 0;
            while (m % i == 0){
                ++ cnt;
                m /= i;
            }
            vec.push_back({i, cnt});
        }
    }
    if (m > 1){
        vec.push_back({m, 1});
    }
    -- n;
    for (int i = 0; i <= n; ++ i){
        scanf("%d", &a[i]);
    }
    pii ans = {0, 1};
    for (auto u : vec){
        p = u.first, e = u.second;
        int x = solve(n, l, r, b);
        ans = crt(ans, {x, moder});
    }
    printf("%d\n", ans.first);
    return 0;
}
