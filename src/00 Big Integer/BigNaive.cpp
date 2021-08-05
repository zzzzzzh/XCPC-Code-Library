#include<bits/stdc++.h>

typedef long long ll;
typedef __int128 i128;

struct Big{ // 仅支持非负整数
    static const int exp = 18;
    static const ll base = 1000000000000000000ll; // 修改它时记得修改输入输出格式 

    std::vector <ll> a;
    int len;
    
    Big():len(0){}
    Big(ll len, int mark):len(len){a.resize(len);}
    
    explicit Big(ll p){
        for ( ; p; p /= base){
            a.push_back(p % base);
        }
        len = a.size();
    }

    Big(char *s, int len){
        ll x = 0, now = 1;
        for (int i = 0; i < len; ++ i){
            x += (s[len - i - 1] - '0') * now;
            if (i % exp == exp - 1){
                a.push_back(x);
                x = 0;
                now = 1;
            }
            else{
                now *= 10;
            }
        }
        if (x){
            a.push_back(x);
        }
        this -> len = a.size();
        normal();
    }
    
    ll operator [] (int sit)const{return sit >= len || sit < 0 ? 0 : a[sit];}
    
    void normal(){
        for ( ; len && !a[len - 1]; -- len)
            ;
    }
    
    bool operator < (const Big &q)const{
        if (len != q.len) return len < q.len;
        for (int i = len - 1; ~i; -- i){
            if (a[i] != q.a[i]) return a[i] < q.a[i];
        }
        return false;
    }
    
    Big operator + (const Big &p)const{
        Big ret(std::max(len, p.len) + 1, -1);
        for (int i = 0; i < ret.len - 1; ++ i){
            ret.a[i] += (*this)[i] + p[i];
            if (ret.a[i] >= base) ret.a[i] -= base, ++ ret.a[i + 1];
        }
        ret.normal();
        return ret;
    }
    
    Big operator - (const Big &p)const{
        Big ret(std::max(len, p.len) + 1, -1);
        for (int i = 0; i < ret.len - 1; ++ i){
            ret.a[i] += (*this)[i] - p[i];
            if (ret.a[i] < 0) ret.a[i] += base, -- ret.a[i + 1];
        }
        ret.normal();
        return ret;
    }
    
    Big operator * (const Big &p)const{ // 注意长度太长可能会爆 __int128，视情况需要将 18 位调小 
        std::vector <i128> vec(len + p.len);
        for (int i = 0; i < len; ++ i){
            for (int j = 0; j < p.len; ++ j){
                vec[i + j] += (i128) a[i] * p.a[j];
            }
        }
        Big ret(len + p.len, -1);
        for (int i = 0; i < ret.len; ++ i){
            ret.a[i] = vec[i] % base;
            vec[i + 1] += vec[i] / base;
        }
        ret.normal();
        return ret;
    }
    
    Big operator * (ll p)const{
        if (!len || !p) return Big();
        i128 remain = 0;
        Big ret;
        for (int i = 0; i < len || remain; ++ i){
            remain += (i128) (*this)[i] * p;
            ret.a.push_back(remain % base);
            remain /= base;
        }
        ret.len = ret.a.size();
        return ret;
    }

    static ll divide(Big &b, const ll &p){
        assert(p);
        i128 remain = 0;
        for (int i = b.len - 1; ~i; -- i){
            remain = remain * base + b.a[i];
            b.a[i] = remain / p;
            remain %= p;
        }
        b.normal();
        return remain;
    }

    Big operator / (const ll &p)const{
        Big ret = *this;
        divide(ret, p);
        return ret;
    }

    ll operator % (const ll &p)const{
        Big ret = *this;
        return divide(ret, p);
    }

    static Big divide(Big &b, const Big &p){
        assert(p.len);
        if (b.len < p.len) return Big();
        b.a.resize(b.len + 1);
        Big ret(b.len - p.len + 1, -1);
        for (int i = b.len - p.len; i >= 0; -- i){
            i128 num = 0, den = 0;
            for (int j = 0; j <= 2; ++ j){
                num = num * base + b[i + p.len - j];
            }
            for (int j = 1; j <= 2; ++ j){
                den = den * base + p[p.len - j];
            }
            ll x = num / den;
            if (!x) continue;
            i128 remain = 0;
            for (int j = 0; j < p.len; ++ j){
                remain = remain + b.a[i + j] - (i128) p.a[j] * x;
                b.a[i + j] = remain % base;
                remain /= base;
                if (b.a[i + j] < 0) b.a[i + j] += base, -- remain;
            }
            b.a[i + p.len] += remain;
            if (b.a[i + p.len] < 0){
                -- x;
                for (int j = 0; j < p.len; ++ j){
                    b.a[i + j] += p.a[j];
                    if (b.a[i + j] >= base) b.a[i + j] -= base, ++ b.a[i + j + 1];
                }
            }
            ret.a[i] = x;
        }
        b.normal();
        ret.normal();
        return ret;
    }

    Big operator / (const Big &p)const{
        Big ret = *this;
        return divide(ret, p);
    }
    
    Big operator % (const Big &p)const{
        Big ret = *this;
        divide(ret, p);
        return ret;
    }

    void write(){
        if (!len) return (void) printf("0");
        printf("%lld", a[len - 1]);
        for (int i = len - 2; ~i; -- i){
            printf("%018lld", a[i]);
        }
    }
};

int main(){
}
