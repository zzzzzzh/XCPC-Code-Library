namespace linear_seq {
	ll res[N], base[N], _c[N], _md[N];
	std::vector<int> Md;
	void mul(ll *a, ll *b, int k){
		rep(i, 0, k + k) _c[i] = 0;
		rep(i, 0, k) if(a[i]) rep(j, 0, k) 
			_c[i + j] = (_c[i + j] + a[i] * b[j]) % mod;
		for(int i = k + k - 1; i >= k; --i) if(_c[i])
			rep(j, 0, SZ(Md)) 
				(_c[i-k+Md[j]] -= _c[i]*_md[Md[j]]) %= mod;
		rep(i, 0, k) a[i] = _c[i];
	}
	int solve(ll n, std::vector<int> a, std::vector<int> b){
		ll ans = 0, pnt = 0; int k = SZ(a);
		assert(SZ(a) == SZ(b));
		rep(i, 0, k) _md[k - 1 - i] = - a[i]; _md[k] = 1;
		Md.clear();
		rep(i, 0, k) if(_md[i]) Md.push_back(i);
		rep(i, 0, k) res[i] = base[i] = 0;
		res[0] = 1;
		while((1ll << pnt) <= n) ++pnt;
		for(int p = pnt; p >= 0; --p){
			mul(res, res, k);
			if((n >> p) & 1){
				for(int i=k-1; i>=0; --i) res[i + 1] = res[i]; 
				res[0] = 0;
				rep(j, 0, SZ(Md)) 
					res[Md[j]]=(res[Md[j]]-res[k]*_md[Md[j]])%mod;
			}
		}
		rep(i, 0, k) ans = (ans + res[i] * b[i]) % mod;
		if(ans < 0) ans += mod;
		return ans;
	}
	std::vector<int> BM(std::vector<int> s){
		std::vector<int> C(1, 1), B(1, 1);
		int L = 0, m = 1, b = 1;
		rep(n, 0, SZ(s)){
			ll d = 0;
			rep(i, 0, L+1) d=(d+1ll*C[i]*s[n - i]) % mod;
			if(d == 0) ++ m;
			else if(2 * L <= n){
				std::vector<int> T = C;
				ll c = mod - d * powmod(b, mod - 2) % mod;
				while(SZ(C) < SZ(B) + m) C.push_back(0);
				rep(i, 0, SZ(B)) C[i+m]=(C[i+m] + c*B[i]) % mod;
				L = n + 1 - L; B = T; b = d; m = 1;
			} else{
				ll c = mod - d * powmod(b, mod - 2) % mod;
				while(SZ(C) < SZ(B) + m) C.push_back(0);
				rep(i, 0, SZ(B)) C[i+m]=(C[i+m] + c*B[i]) % mod;
				++ m;
			}
		}
		return C;
	}
	int gao(std::vector<int> a, ll n){
		std::vector<int> c = BM(a);
		c.erase(c.begin());
		rep(i, 0, SZ(c)) c[i] = (mod - c[i]) % mod;
		return solve(n,c,vi(a.begin(),a.begin()+SZ(c)));
	}
}
