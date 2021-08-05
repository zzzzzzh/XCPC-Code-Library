<TeX>搞了一个cf上必没法hack的hash板子。本板子因为封装较深，有些卡常，使用时请务必注意</TeX>
#include <bits/stdc++.h>

using pii = std::pair <int, int>;

struct Hash{
    int moder, base;
    std::vector <int> power;
    std::vector <int> hash;

    Hash(int moder, int base, const char *s, int n):moder(moder), base(base), power(n + 1), hash(n + 1){
        hash[0] = 0, power[0] = 1;
        for (int i = 1; i <= n; ++ i){
            hash[i] = (1ll * base * hash[i - 1] + s[i - 1]) % moder;
            power[i] = 1ll * power[i - 1] * base % moder;
        }
    }

    int query(int l, int r){
        int ret = (hash[r + 1] - 1ll * power[r - l + 1] * hash[l]) % moder;
        ret += ret < 0 ? moder : 0;
        return ret;
    }
};

struct Hasher{
    std::vector<Hash> hashes1, hashes2;

    Hasher(const std::vector<pii> &pbs, char *s, char *t){
        int n = strlen(s), m = strlen(t);
        for (auto pb : pbs){
            hashes1.emplace_back(pb.first, pb.second, s, n);
            hashes2.emplace_back(pb.first, pb.second, t, m);
        }
    }

    bool query(int l1, int r1, int l2, int r2){
        int sz = hashes1.size();
        for (int i = 0; i < sz; ++ i){
            if (hashes1[i].query(l1, r1) != hashes2[i].query(l2, r2)){
                return false;
            }
        }
        return true;
    }
};

int main(){
    std::vector<int> primes = {
        1001452733,
        1001452759,
        1001452787,
        1001452789,
        1001452871,
        1001452897,
        1001452937,
        1001452957,
        1001452981,
        1001452993,
        1001452999,
        1001453021,
        1001453029,
        1001453041,
        1001453069,
        1001453081
    };

    std::vector<int> bases = {
        37,
        41,
        43,
        47,
        53,
        59,
        61,
        67,
        71,
        73,
        79,
        83,
        89,
        97,
        101,
        103,
    };

    std::mt19937 rnd(std::chrono::steady_clock.now().time_since_epoch().count());
    std::shuffle(primes.begin(), primes.end(), rnd);
    std::shuffle(bases.begin(), bases.end(), rnd);
    return 0;
}
