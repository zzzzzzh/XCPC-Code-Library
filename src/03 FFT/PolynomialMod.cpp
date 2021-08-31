<TeX>
包含了多项式乘法，逆，除法，对数，指数（前面的复杂度是 $\mathcal{O}(n\log n)$），多点求值，插值（前面的复杂度是 $\mathcal{O}(n\log^{2}(n))$）。
der()指求导，integral()指积分。
</TeX>
struct poly{
	std::vector<int> a; int len; poly() : len(-1) {}
	poly(int len) : len(len) { a.resize(len + 1); }
	poly(const poly &p, int len) : len(len) { 
		a.resize(len + 1); 
		for (int i = 0; i <= len; ++i)
			a[i] = i > p.len ? 0 : p.a[i];
	}
	int &operator [] (int n) { assert(n >= 0 && n <= len); return a[n]; }
	void setlen(int len) {
		a.resize(len + 1); this->len = len;
	}
    // 相当于乘以 x ^ dis
    poly operator << (const int &dis) const {
        poly ret(len + dis);
        std::copy(a.begin(), a.begin() + len + 1, ret.a.begin() + dis);
        return ret;
    }
    // 相当于除以 x ^ dis
    poly operator >> (const int &dis) const {
        if (dis > len) return poly(-1);
        int retlen = len - dis;
        poly ret(retlen);
        std::copy(a.begin(), a.begin() + retlen + 1, ret.a.begin());
        return ret;
    }
    int value(int x) {
        int now = 1, ret = 0;
        for (int i = 0; i <= len; ++i) {
            ret = (ret + 1ll * a[i] * now) % moder;
            now = 1ll * now * x % moder;
        }
        return ret;
    }
    poly operator + (const poly &p) const {
        poly ret(*this, std::max(len, p.len));
        for (int i = 0; i <= p.len; ++i) {
            ret.a[i] += p.a[i];
            ret.a[i] -= ret.a[i] >= moder ? moder : 0;
        }
        for ( ; ~ret.len && !ret.a[ret.len]; --ret.len)
            ;
        return ret;
    }

    poly operator - (const poly &p) const {
        poly ret(*this, std::max(len, p.len));
        for (int i = 0; i <= p.len; ++i) {
            ret.a[i] -= p.a[i];
            ret.a[i] += ret.a[i] < 0 ? moder : 0;
        }
        for ( ; ~ret.len && !ret.a[ret.len]; --ret.len)
            ;
        return ret;
    }

    poly operator - () const {
        poly ret(len);
        for (int i = 0; i <= len; ++i){
            ret.a[i] = a[i] ? moder - a[i] : 0;
        }
        return ret;
    }
    poly operator * (const poly &p) const {
        if (!~len || !~p.len) return poly(-1);
        int n = len + p.len, lenret = 1;
        for ( ; lenret <= n; lenret <<= 1);
        poly ret(*this, lenret);
        std::vector<int> aux(lenret);
		std::copy(p.a.begin(), p.a.begin() + p.len + 1, aux.begin());
		NTT(ret.a, lenret, 0); NTT(aux, lenret, 0);
		for (int i = 0; i < lenret; ++i)
			ret.a[i] = 1ll * ret.a[i] * aux[i] % moder;
		NTT(ret.a, lenret, 1); ret.len = n;
		return ret;
	}
	//表示求最高次为n的inv，log和exp同理
	poly inv(int n) const {
		assert(~len && a[0]);
		poly ret(0);
		ret.a[0] = powermod(a[0], moder - 2);
		for (int noweps = 0; noweps < n; ) {
			noweps = noweps << 1 | 1;
			poly aux = poly(*this, noweps) * (ret * ret);
			aux.setlen(noweps);
			ret = ret + ret - aux;
		}
		ret.setlen(n);
		return ret;
	}
	poly operator / (const poly &p) const {
		assert(~p.len);
		if (p.len > len) return poly(-1);
		poly a(*this), b(p);
		std::reverse(a.a.begin(), a.a.begin() + a.len + 1);
		std::reverse(b.a.begin(), b.a.begin() + b.len + 1);
		poly ret(b.inv(len - p.len));
		ret *= a;
		int retlen = len - p.len;
		ret.setlen(retlen);
		std::reverse(ret.a.begin(), ret.a.begin()+retlen+1);
		return ret;
	}
	poly log(int n) const {
		assert(~len && a[0] == 1);
		poly aux(*this, n), ret = aux.der() * aux.inv(n - 1);
		ret.setlen(n - 1);
		return ret.integral();
	}
	poly exp(int n) const {
		assert(!~len || !a[0]);
		poly ret(0);
		ret.a[0] = 1;
		poly unit = ret;
		for (int noweps = 0; noweps < n; ){
			noweps = noweps << 1 | 1;
			ret *= unit - ret.log(noweps) + poly(*this, noweps);
			ret.setlen(noweps);
		}
		ret.setlen(n);
		return ret;
	}
};
poly aux[N];
void dfs(int l, int r, int id, std::vector <int> &vec){
	if (l == r){
		aux[id] = poly(1);
		aux[id][1] = 1;
		aux[id][0] = vec[l] ? moder - vec[l] : 0;
		return;
	}
	int mid = (l + r) >> 1;
	dfs(l, mid, id << 1, vec);
	dfs(mid + 1, r, id << 1 | 1, vec);
	aux[id] = aux[id << 1] * aux[id << 1 | 1];
}
std::vector <int> solve1(int l, int r, int id, poly p){
    if (l == r) return {p.len == -1 ? 0 : p[0]};
	int mid = (l + r) >> 1;
	vi vec1 = solve1(l, mid, id << 1, p % aux[id << 1]);
	vi vec2 = solve1(mid+1, r, id<<1|1, p % aux[id<<1|1]);
	vec1.insert(vec1.end(), vec2.begin(), vec2.end());
	return vec1;
}
std::vector<int> multivalue(poly p, std::vector<int> vec){
	int n = vec.size(); dfs(0, n - 1, 1, vec);
	p = p % aux[1];
	return solve1(0, n - 1, 1, p);
}
poly solve2(int l, int r, int id, std::vector <int> &vec){
	if (l == r){
		poly p(0); p[0] = vec[l];
		return p;
	}
	int mid = (l + r) >> 1;
	poly p1 = solve2(l, mid, id << 1, vec);
	poly p2 = solve2(mid + 1, r, id << 1 | 1, vec);
	return p1 * aux[id << 1 | 1] + p2 * aux[id << 1];
}
poly interpolation(vi vecx, vivecy){
	int n = vecx.size() - 1;
	dfs(0, n, 1, vecx);
	poly p = aux[1].der();
	std::vector <int> vec = multivalue(p, vecx);
	for (int i = 0; i <= n; ++ i)
		vec[i] = 1ll * vecy[i] * pmod(vec[i], mod - 2) % mod;
	return solve2(0, n, 1, vec);
}
