<TeX>这年头谁还用cpp写大整数啊...</TeX>
// 大整数模板 
// []运算符类似python，负数从右边开始取

#include<bits/stdc++.h>
#include"ntt.cpp"

struct Big{
    static const int base = 10;

    std::vector <int> a;
    int len, sig;
    
    Big():len(0), sig(0){}
    Big(int len, int sig):len(len), sig(sig){a.resize(len);}

    explicit Big(ll p):len(0), sig((p > 0) - (p < 0)){
        for (ll x = std::abs(p); x; x /= base){
            a.push_back(x % base);
            ++ len;
        }
    }

    Big(const Big &b, const int &len):len(len), sig(b.sig){
        a.resize(len);
        for (int i = 0; i < len; ++ i){
            a[i] = i < b.len ? b.a[i] : 0;
        }
    }

    int operator [](int n)const{return n >= len || n < -len ? 0 : a[n + (n < 0 ? len : 0)];}

    void normal(){
        for ( ; len && !a[len - 1]; -- len)
            ;
        if (!len) sig = 0;
    }

    ll toint(){
        ll ret = 0;
        for (int i = len - 1; i >= 0; -- i){
            ret = ret * 10 + a[i];
        }
        return ret;
    }

    void read(){
        a.clear();
        len = 0;
        char ch;
        while (!isdigit(ch = getchar()) && ch != '-')
            ;
        if (ch == '-'){
            sig = -1;
        }
        else{
            sig = 1;
            a.push_back(ch - '0');
        }
        while (isdigit(ch = getchar())){
            a.push_back(ch - '0');
        }
        len = a.size();
        std::reverse(a.begin(), a.end());
        normal();
        if (!len) sig = 0;
    }

    void write(){
        if (!sig) return (void) putchar('0');
        if (sig < 0) putchar('-');
        for (int i = len - 1; i >= 0; -- i){
            putchar(a[i] + '0');
        }
    }
    
    bool absge(const Big &q)const{
        if (len != q.len) return len > q.len;
        for (int i = len - 1; ~i; -- i){
            if (a[i] != q.a[i]) return a[i] > q.a[i];
        }
        return true;
    }
    
    static int divide(Big &p, const int &q){
        assert(q);
        if (!p.sig) return 0;
        ll remain = 0, x = std::abs(q);
        for (int i = p.len - 1; ~i; -- i){
            remain = remain * p.base + p.a[i];
            p.a[i] = remain / x;
            remain %= x;
        }
        remain *= p.sig;
        p.normal();
        p.sig *= q < 0 ? -1 : 1;
        return remain;
    }

    bool operator == (const Big &p)const{
        if (sig != p.sig || len != p.len) return false;
        for (int i = 0; i < len; ++ i){
            if (a[i] != p.a[i]) return false;
        }
        return true;
    }
    
    bool operator < (const Big &p)const{
        if (sig != p.sig) return sig < p.sig;
        if (len != p.len) return (len < p.len) ^ (sig == -1);
        for (int i = len - 1; i >= 0; -- i){
            if (a[i] != p.a[i]) return (a[i] < p.a[i]) ^ (sig == -1);
        }
        return false;
    }
    
    // 右移和左移均为移动该大整数的进制位 
    Big operator << (const int &dis)const{
        if (!sig) return *this;
        Big ret(len + dis, sig);
        std::copy(a.begin(), a.begin() + len, ret.a.begin() + dis);
        return ret;
    }
    
    Big operator >>(const int &dis)const{
        if (dis >= len) return Big();
        Big ret(len - dis, sig);
        std::copy(a.begin() + dis, a.begin() + len, ret.a.begin()); return ret;
        return ret;
    }
    
    Big operator + (const Big &p)const{
        if (!p.sig) return *this;
        if (!sig) return p;
        bool type = true;
        const Big *aux = this, *aux1 = &p;
        if (sig != p.sig){
            type = false;
            if (!absge(p)){
                std::swap(aux, aux1);
            }
        }
        Big ret(*aux, std::max(len, p.len) + 1);
        for (int i = 0; i < ret.len - 1; ++ i){
            if (type){
                ret.a[i] += (*aux1)[i];
                if (ret.a[i] >= base) ret.a[i] -= base, ++ ret.a[i + 1];
            }
            else{
                ret.a[i] -= (*aux1)[i];
                if (ret.a[i] < 0) ret.a[i] += base, -- ret.a[i + 1];
            }
        }
        ret.normal();
        return ret;
    }
    
    Big operator - ()const{
        Big ret(*this);
        ret.sig = -ret.sig;
        return ret;
    }
    
    Big operator - (const Big &p)const{
        return *this + (-p);
    }

    Big &operator ++ (){return *this = *this + Big(1);}
    Big &operator -- (){return *this = *this - Big(1);}
    
    Big operator * (const Big &p)const{
        if (!sig || !p.sig) return Big();
        int n = len + p.len;
        int lenret = 1;
        for ( ; lenret < n; lenret <<= 1)
            ;
        Big ret(*this, lenret);
        std::vector <int> aux(lenret);
        std::copy(p.a.begin(), p.a.begin() + p.len, aux.begin());
        NTT(ret.a, lenret, 0);
        NTT(aux, lenret, 0);
        for (int i = 0; i < lenret; ++ i){
            ret.a[i] = 1ll * ret.a[i] * aux[i] % moder;
        }
        NTT(ret.a, lenret, 1);
        for (int i = 0; i < n - 1; ++ i){
            ret.a[i + 1] += ret.a[i] / base;
            ret.a[i] %= base;
        }
        ret.len = n;
        ret.normal();
        ret.sig = sig * p.sig;
        return ret;
    }

    Big operator * (const int &p)const{
        if (!p || !sig) return Big();
        Big ret;
        ll x = std::abs(p), remain = 0;
        for (int i = 0; i < len || remain; ++ i){
            remain += (*this)[i] * x;
            ret.push_back(remain % base);
            remain /= base;
        }
        ret.len = ret.a.size();
        ret.sig = sig * (p < 0 ? -1 : 1);
        return ret;
    }
    
    Big operator / (const Big &p)const &{
        assert(p.sig);
        if (!sig || len < p.len) return Big();
        ll num = 0, x = 1;
        for (int i = 1; i <= 3; ++ i){
            num = num * base + p[-i];
        }
        for (int i = 0; i < 5; ++ i){
            x *= base;
        }
        Big ret(x / num), aux = *this, aux1 = p;
        aux.sig = aux1.sig = 1;
        int noweps = 1;
        for ( ; noweps <= len - p.len; noweps <<= 1){
            Big aux2((noweps << 1) + 3, 1);
            for (int i = std::max(0, p.len - aux2.len); i < p.len; ++ i){
                aux2.a[i - p.len + aux2.len] = p[i];
            }
            aux2 = (((aux2 * ret) >> (noweps + 2)) * ret) >> (noweps + 2);
            ret = ((ret + ret) << noweps) - aux2;
        }
        ret = (ret * aux) >> (p.len + noweps + 1);
        if (!absge(ret * aux1)) -- ret;
        else if (!absge(++ ret * aux1)) -- ret; 
        ret.sig *= sig * p.sig;
        return ret;
    }
    
    Big operator / (const int &p)const{
        Big ret(*this);
        divide(ret, p);
        return ret;
    }
    
    Big sqrt()const{
        assert(sig >= 0);
        if (!sig) return *this;
        ll num = 0, x = 1;
        for (int i = len - 1; i >= len - 8; -- i){
            num = num * base + (i >= 0 ? a[i] : 0);
        }
        for (int i = 0, sz = len & 1 ? 13 : 14; i < sz; ++ i){
            x *= base;
        }
        Big ret((ll) std::sqrt(x / num));
        int noweps = 2;
        for ( ; noweps <= (len >> 1) + 1; noweps = (noweps << 1) - 1){
            Big aux((noweps << 1) + 1 + (len & 1), 1);
            for (int i = std::max(0, len - aux.len); i < len; ++ i){
                aux.a[i - len + aux.len] = a[i];
            }
            aux = ((((aux * ret) >> (noweps + 1)) * ret) >> (noweps + 1)) / 2;
            Big aux1((noweps + 1) << 1, 1);
            aux1.a[aux1.len - 1] = 1, aux1.a[aux1.len - 2] = 5;
            ret = (ret * (aux1 - aux)) >> (noweps + 2);
        }
        ret = (ret * *this) >> ((len >> 1) + noweps + 1);
        if (!absge(ret * ret)) -- ret;
        else {++ ret; if (!absge(ret * ret)) -- ret;}
        return ret;
    }
    
    Big operator % (const Big &p)const{
        assert(p.sig);
        return *this - *this / p * p;
    }
    
    int operator % (const int &p)const{
        assert(p);
        Big aux(*this);
        return divide(aux, p);
    }
};

int main(){
    return 0;
}
