namespace BM {
    const int N = 5010;
    ll res[N], base[N], _c[N], _md[N];
    std::vector<int> Md;

    void mul(ll *a, ll *b, int k) {
        memset(_c, 0, sizeof(ll) * k * 2);
        for (int i = 0; i < k; ++i)
            if (a[i])
                for (int j = 0; j < k; ++j)
                    _c[i + j] = (_c[i + j] + a[i] * b[j]) % moder;
        for (int i = 2 * k - 2; i >= k; --i)
            if (_c[i])
                for (int j = 0; j < Md.size(); ++j)
                    (_c[i - k + Md[j]] -= _c[i] * _md[Md[j]]) %= moder;
        for (int i = 0; i < k; ++i) a[i] = _c[i];
    }

    int solve(ll n, std::vector<int> a, std::vector<int> b) {
        ll ans = 0, pnt = 0;
        int k = a.size() - 1;
        assert(a.size() - 1 == b.size());
        for (int i = 0; i <= k; ++i) _md[k - i] = a[i];
        Md.clear();
        for (int i = 0; i < k; ++i) if (_md[i]) Md.push_back(i);
        for (int i = 0; i < k; ++i) res[i] = base[i] = 0;
        res[0] = 1;
        while ((1ll << pnt) <= n) ++pnt;
        for (int p = pnt; p >= 0; --p) {
            mul(res, res, k);
            if ((n >> p) & 1) {
                for (int i = k - 1; i >= 0; --i) res[i + 1] = res[i];
                res[0] = 0;
                for (int j = 0; j < Md.size(); ++j) {
                    res[Md[j]] = (res[Md[j]] - res[k] * _md[Md[j]]) % moder;
                }
            }
        }
        for (int i = 0; i < k; ++i) {
            ans = (ans + res[i] * b[i]) % moder;
        }
        ans += ans < 0 ? moder : 0;
        return ans;
    }

    std::vector<int> BM(std::vector<int> s) {
        std::vector<int> C(1, 1), B(1, 1);
        int l = 0, m = 1, b = 1;
        for (int n = 0; n < s.size(); ++n) {
            ll d = 0;
            for (int i = 0; i <= l; ++i) {
                d = (d + 1ll * C[i] * s[n - i]) % moder;
            }
            if (!d) ++m;
            else {
                std::vector<int> T = C;
                ll c = moder - d * powermod(b, moder - 2) % moder;
                while (C.size() < B.size() + m) C.push_back(0);
                for (int i = 0; i < B.size(); ++i) {
                    C[i + m] = (C[i + m] + c * B[i]) % moder;
                }
                if (l * 2 <= n) {
                    l = n + 1 - l, B = T, b = d, m = 1;
                } else {
                    ++m;
                }
            }
        }
        return C;
    }
}
