#include<bits/stdc++.h>

typedef long long ll;
typedef std::pair <int, int> pii;

int powermod(int a, int exp, int moder){
    int ret = 1;
    for ( ; exp > 0; exp >>= 1){
        if (exp & 1){
            ret = 1ll * ret * a % moder;
        }
        a = 1ll * a * a % moder;
    }
    return ret;
}

struct poly{
    static const int N = 1000;

    int a[N];
    int length, moder;

    poly():length(-1), moder(0){memset(a, 0, sizeof(a));}
    poly(int moder):length(-1), moder(moder){memset(a, 0, sizeof(a));}

    int value(int x){
        int ret = 0;
        for (int i = 0, now = 1; i <= length; ++ i){
            ret = (ret + 1ll * a[i] * now) % moder;
            now = 1ll * now * x % moder;
        }
        return ret;
    }

    int &operator [](int sit){return a[sit];}

    poly operator + (const poly &p)const{
        poly ret(moder);
        ret.length = std::max(length, p.length);
        for (int i = 0; i <= ret.length; ++ i){
            ret.a[i] = a[i] + p.a[i];
            ret.a[i] -= ret.a[i] >= moder ? moder : 0;
        }
        for ( ; ~ret.length && !ret.a[ret.length]; -- ret.length)
            ;
        return ret;
    }

    poly operator - ()const{
        poly ret(moder);
        ret.length = length;
        for (int i = 0; i <= ret.length; ++ i){
            ret.a[i] = a[i] ? moder - a[i] : 0;
        }
        return ret;
    }

    poly operator - (const poly &p)const{
        poly ret(moder);
        ret.length = std::max(length, p.length);
        for (int i = 0; i <= ret.length; ++ i){
            ret.a[i] = a[i] - p.a[i];
            ret.a[i] -= ret.a[i] < 0 ? moder : 0;
        }
        for ( ; ~ret.length && !ret.a[ret.length]; -- ret.length)
            ;
        return ret;
    }

    poly operator * (const poly &p)const{
        poly ret(moder);
        if (!~length || !~p.length) return ret;
        ret.length = length + p.length;
        for (int i = 0; i <= length; ++ i){
            for (int j = 0; j <= p.length; ++ j){
                ret.a[i + j] = (ret.a[i + j] + 1ll * a[i] * p.a[j]) % moder;
            }
        }
        return ret;
    }

    poly operator * (const int &p)const{
        poly ret(moder);
        ret.length = length;
        for (int i = 0; i <= ret.length; ++ i){
            ret.a[i] = 1ll * a[i] * p % moder;
        }
        return ret;
    }

    poly operator / (const poly &p)const{
        poly ret(moder);
        if (!~p.length) assert(("division by zero!", 0));
        if (length < p.length) return ret;
        ret.length = length - p.length;
        poly aux = *this;
        while (aux.length >= p.length){
            int x = 1ll * aux.a[aux.length] * powermod(p.a[p.length], moder - 2, moder) % moder;
            ret.a[aux.length - p.length] = x;
            for (int j = 0; j <= p.length; ++ j){
                int &u = aux.a[aux.length + j - p.length];
                u = (u - 1ll * x * p.a[j]) % moder;
                u += u < 0 ? moder : 0;
            }
            for ( ; ~aux.length && !aux.a[aux.length]; -- aux.length)
                ;
        }
        return ret;
    }

    poly operator % (const poly &p)const{
        if (!~p.length) assert(("division by zero!", 0));
        poly ret = p;
        while (ret.length >= p.length){
            int x = 1ll * ret.a[ret.length] * powermod(p.a[p.length], moder - 2, moder) % moder;
            for (int j = 0; j <= p.length; ++ j){
                int &u = ret.a[ret.length + j - p.length];
                u = (u - 1ll * x * p.a[j]) % moder;
                u += u < 0 ? moder : 0;
            }
            for ( ; ~ret.length && !ret.a[ret.length]; -- ret.length)
                ;
        }
        return ret;
    }
};

std::vector <int> interpolation(std::vector <int> x, std::vector <int> y){
    int n = (int) x.size() - 1;
    std::vector <int> dp(n + 2);
    dp[0] = 1;
    for (int i = 0; i <= n; ++ i){
        for (int j = i + 1; j >= 0; -- j){
            dp[j] = (1ll * dp[j] * -x[i] + (j ? dp[j - 1] : 0)) % moder;
        }
    }
    std::vector <int> ret(n + 1);
    for (int i = 0; i <= n; ++ i){
        int coe = 1, coe1 = 0;
        for (int j = 0; j <= n; ++ j){
            if (i != j) coe = 1ll * coe * (x[i] - x[j]) % moder;
        }
        coe = 1ll * powermod(coe, moder - 2) * y[i] % moder;
        for (int j = n; j >= 0; -- j){
            coe1 = (dp[j + 1] + 1ll * coe1 * x[i]) % moder;
            ret[j] = (ret[j] + 1ll * coe * coe1) % moder;
        }
    }
    for (int i = 0; i <= n; ++ i){
        ret[i] += ret[i] < 0 ? moder : 0;
    }
    return ret;
}
static int interpolation(std::vector <int> vec, int x){
    int n = vec.size() - 1;
    if (x >= 0 && x <= n) return vec[x];
    std::vector <int> pre(n + 1), suf(n + 1);
    pre[0] = suf[n] = 1;
    for (int i = 1; i <= n; ++ i){
        pre[i] = 1ll * pre[i - 1] * (x - i + 1) % moder;
        pre[i] += pre[i] < 0 ? moder : 0;
    }
    for (int i = n - 1; i >= 0; -- i){
        suf[i] = 1ll * suf[i + 1] * (x - i - 1) % moder;
        suf[i] += suf[i] < 0 ? moder : 0;
    }
    int ans = 0;
    for (int i = 0; i <= n; ++ i)
        ans = (ans + ((n - i) & 1 ? -1ll : 1ll) * vec[i] * pre[i] % moder * suf[i] % moder * invf[i] % moder * invf[n - i]) % moder;
    ans += ans < 0 ? moder : 0;
    return ans;
}

int main(){
}
