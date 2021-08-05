comp w[2][1 << MAX];
void init(){
	for (int i = 0; i < 1 << MAX; ++i){
		double angle = 1.0 * i / (1 << (MAX - 1)) * PI;
		for (int type = -1; type <= 1; type += 2)
			w[!~type][i] = comp(cos(angle), type * sin(angle));
	}
}
void FFT(std::vector <comp> &a, int len, int type){
	for (int i = 1, j = 0; i < len - 1; ++i){
		for (int s = len; j ^= s >>= 1, ~j & s; ) ;
		if (i < j) std::swap(a[i], a[j]);
	}
	for (int i = 2, d = 1; i <= len; i <<= 1, ++d)
		for (int j = 0; j < len; j += i){
			auto u = a.begin() + j;
			for (int k = 0; k < i / 2; ++k){
				comp s = u[k];
				comp t = w[type][k << (MAX - d)] * u[k + i / 2];
				u[k] = s + t, u[k + i / 2] = s - t;
			}
		}
	if (type == 0) return; double inv = 1.0 / len;
	for (int i = 0; i < len; ++i) a[i] = a[i] * inv;
}
void FFTcomb(std::vector<comp> &a, std::vector<comp> &b){
	int n = a.size(); FFT(a, n, 0);
	for (int i = 0; i < n; ++i) b[i] = a[i?n-i:0].conj();
	for (int i = 0; i < n; ++i){
		comp tmp1 = (a[i] + b[i]) * comp(0.5, 0);
		comp tmp2 = (a[i] - b[i]) * comp(0, -0.5);
		a[i] = tmp1, b[i] = tmp2;
	}
}
poly operator * (const poly &p)const{
	int mod = std::sqrt(moder) + 2;
	int n = len + p.len, lenret = 1;
	for ( ; lenret <= n; lenret <<= 1);
	std::vector <comp> aux(lenret), aux1(lenret), aux2(lenret), aux3(lenret);
	for (int i = 0; i <= len; ++ i)
		aux[i] = comp(a[i] / mod, a[i] % mod);
	for (int i = 0; i <= p.len; ++ i)
		aux2[i] = comp(p.a[i] / mod, p.a[i] % mod);
	FFTcomb(aux, aux1); FFTcomb(aux2, aux3);
	for (int i = 0; i < lenret; ++ i){
		comp tmp1=aux[i]*aux2[i] + comp(0,1)*aux1[i]*aux3[i];
		comp tmp2=aux1[i]*aux2[i] + comp(0,1)*aux[i]*aux3[i];
		aux[i] = tmp1, aux1[i] = tmp2;
	}
	FFT(aux, lenret, 1); FFT(aux1, lenret, 1);
	poly ret(n);
	for (int i = 0; i <= n; ++ i){
		int x = (ll) std::round(aux[i].real) % moder;
		int y = (ll) std::round(aux[i].imag) % moder;
		int u = (ll) std::round(aux1[i].real) % moder;
		int v = (ll) std::round(aux1[i].imag) % moder;
		ret.a[i] = (1ll * x * mod * mod + y + 1ll * (u + v) * mod) % moder;
	}
	return ret;
}
