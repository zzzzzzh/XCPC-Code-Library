<TeX>操作数之一 $\ge 65536$ 时，结果就有可能超出 int 范围</TeX>
const int NIMN = 256;
int threshold = 2;
ll prod[NIMN][NIMN];
ll nimprodpow(ll x, ll y){
	if (x < threshold && y < threshold) return prod[x][y];
	ll a = 0, max = std::max(x, y);
	for ( ; 1ll << (1 << a) <= max; ++ a);
	a = 1 << a - 1;
	ll p = x >> a, s = y >> a, t = y & (1 << a) - 1;
	ll ret = nimprodpow(p, s);
	return (ret ^ nimprodpow(p, t)) << a ^ nimprodpow(1ll << a - 1, ret);
}
ll nimprod(ll x, ll y){
	if (x < threshold && y < threshold) return prod[x][y];
	ll a = 0, max = std::max(x, y);
	for ( ; 1ll << (1 << a) <= max; ++ a);
	a = 1 << a - 1;
	ll p = x >> a, q = x & (1ll << a) - 1;
	ll s = y >> a, t = y & (1ll << a) - 1;
	ll ret = nimprod(p, s);
	return (ret ^ nimprod(p, t) ^ nimprod(q, s)) << a ^ nimprodpow(1ll << a - 1, ret) ^ nimprod(q, t);
}
void init(){
	prod[1][1] = 1;
	for (int i = 0; i < NIMN; ++ i)
		for (int j = 0; j < NIMN; ++ j)
			prod[i][j] = nimprod(i, j);
	threshold = NIMN;
}
