namespace RS{
    const int N = 100;

    ll moder;
    std::vector <ll> aold[N], a[N], anew[N];
    std::vector <ll> bold[N], b[N], bnew[N];

    std::vector <ll> RS(std::vector <ll> s, ll p, int e){
        moder = 1;
        int n = s.size();
        for (int i = 0; i < e; ++ i, moder *= p){
            a[i] = anew[i] = {moder};
            b[i] = {}, bnew[i] = {moder * s[0]};
        }
        auto gettheta = [&](ll x, ll &theta, int &u){
            if (!x) theta = 1, u = e;
            else for (u = 0, theta = x; theta % p == 0; theta /= p, ++ u)
                    ;
        };
        std::vector <ll> theta(e);
        std::vector <int> u(e);
        for (int i = 0; i < e; ++ i){
            bnew[i][0] %= moder;
            gettheta(bnew[i][0], theta[i], u[i]);
            if (!bnew[i][0]) bnew[i] = {};
        }
        std::vector <ll> thetaold(e);
        std::vector <int> uold(e), r(e);
        for (int i = 1; i < n; ++ i){
            for (int j = 0; j < e; ++ j){
                if (std::max(a[j].size(), b[j].size() + 1) < std::max(anew[j].size(), bnew[j].size() + 1)){
                    int h = e - 1 - u[j];
                    aold[j] = a[h], uold[j] = u[h], bold[j] = b[h];
                    r[j] = i - 1, thetaold[j] = theta[h];
                }
            }
            for (int j = 0; j < e; ++ j){
                a[j] = anew[j], b[j] = bnew[j];
            }
            for (int j = 0; j < e; ++ j){
                ll x = 0;
                for (int k = 0; k < (int) a[j].size(); ++ k){
                    x = (x + s[i - k] * a[j][k]) % moder;
                }
                gettheta(x, theta[j], u[j]);
                if (!x) continue;
                int g = e - 1 - u[j];
                if ((int) a[g].size() == 1 && b[g].empty()){
                    bnew[j].resize(i + 1);
                    bnew[j][i] = x;
                }
                else{
                    int offset = i - r[g];
                    ll coe = theta[j] * inv(thetaold[g], moder) % moder * a[u[j] - uold[g]][0] % moder;
                    auto update = [&](std::vector <ll> &a, std::vector <ll> &b){
                        unsigned sz = b.size() + offset;
                        if (a.size() < sz) a.resize(sz);
                        for (int i = 0; i < (int) b.size(); ++ i){
                            ll &u = a[i + offset];
                            u = (u - b[i] * coe) % moder;
                            u += u < 0 ? moder : 0;
                        }
                        while (!a.empty() && !a.back()) a.pop_back();
                    };
                    update(anew[j], aold[g]);
                    update(bnew[j], bold[g]);
                }
            }
        }
        anew[0].resize(std::max(anew[0].size(), bnew[0].size() + 1));
        return anew[0];
    }

    std::vector <ll> RS(std::vector <ll> s, std::vector <pii> p) {
        ll tmoder = 1;
        std::vector <ll> ret = {1};
        for (auto u : p){
            std::vector <ll> a = RS(s, u.first, u.second);
            int sz = a.size();
            if ((int) ret.size() < sz) ret.resize(sz);
            for (int i = 0; i < sz; ++ i){
                ret[i] = (ret[i] * inv(moder, tmoder) * moder + a[i] * inv(tmoder, moder) * tmoder) % (moder * tmoder);
            }
            tmoder *= moder;
        }
        return ret;
    }
}
