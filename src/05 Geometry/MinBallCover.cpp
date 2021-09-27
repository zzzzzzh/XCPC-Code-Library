struct poi {
	Float cor[3];
	explicit poi(Float x = 0, Float y = 0, Float z = 0) {
		cor[0] = x, cor[1] = y, cor[2] = z;
	}
	inline poi add(const poi &p) const {
		poi ret;
		rep(i, 3) ret.cor[i]=cor[i]+p.cor[i];//sub,mul,dot
		return ret;
	}
	inline poi det(const poi &p) const {
		poi ret;
		rep(i, 3) {
			int j = i == 2 ? 0 : i + 1;
			int k = j == 2 ? 0 : j + 1;
			ret.cor[i] = cor[j] * p.cor[k] - cor[k] * p.cor[j];
		}
		return ret;
	}
	inline Float abs2() const {return dot(*this);}
} p[max_N];
int n;
Float a[7][7], ans[7];
inline void gauss(int n) {
	for (int i = 0, j, k; i < n; ++i) {
		for (k = j = i; j < n; ++j)
			if (std::abs(a[k][i]) < std::abs(a[j][i])) k = j;
		if (i != k)
			for (j = i; j <= n; ++j) std::swap(a[i][j],a[k][j]);
		for (j = i + 1; j <= n; ++j) a[i][j] /= a[i][i];
		a[i][i] = 1.;
		for (j = 0; j < n; ++j) {
			if (i == j) continue;
			for(k=i+1;k<=n;++k) a[j][k] -= a[j][i] * a[i][k];
			a[j][i] = 0.;
		}
	}
	rep(i, n) ans[i] = a[i][n];
}
using Ball = std::pair<poi, Float>;
inline bool cmp(const Ball &B1, const Ball &B2) {
	return B1.second < B2.second;}
inline Float dis2(const poi &p1, const poi &p2) {
	return p2.sub(p1).abs2();}
inline bool inBall(const Ball &B, const poi &p) {
	return dcmp(dis2(B.first, p) - B.second) <= 0;}
inline Ball Ball2(const poi &p1, const poi &p2) {
	return {p1.add(p2).mul(0.5), dis2(p1, p2) * 0.25};}
Ball Ball3(const poi &p1, const poi &p2, const poi &p3) {
	poi nor = p2.sub(p1).det(p3.sub(p1));
	Float S = nor.abs2();
	if (!dcmp(S))
		return max(max(Ball2(p1,p2),Ball2(p1,p3),cmp),Ball2(p2,p3),cmp);
	rep(i, 3) a[0][i] = 2 * (p2.cor[i] - p1.cor[i]);
	a[0][3] = p2.abs2() - p1.abs2();
	rep(i, 3) a[1][i] = 2 * (p3.cor[i] - p1.cor[i]);
	a[1][3] = p3.abs2() - p1.abs2();
	rep(i, 3) a[2][i] = nor.cor[i];
	a[2][3] = nor.dot(p1);
	gauss(3);
	poi o = poi(ans[0], ans[1], ans[2]);
	Float r2 = dis2(o, p1);
	return {o, r2};
}
Ball Ball4(const poi &p1, &p2, &p3, &p4) {
	poi nor = p2.sub(p1).det(p3.sub(p1));
	Float V = p4.sub(p1).dot(nor);
	if (!dcmp(V)) {
		Ball tmp1=max(Ball3(p1,p2,p3), Ball3(p1,p2,p4), cmp);
		Ball tmp2=max(Ball3(p1,p3,p4), Ball3(p2,p3,p4), cmp);
		return std::max(tmp1, tmp2, cmp);
	}
	rep(i, 3) a[0][i] = 2 * (p2.cor[i] - p1.cor[i]);
	a[0][3] = p2.abs2() - p1.abs2();
	rep(i, 3) a[1][i] = 2 * (p3.cor[i] - p1.cor[i]);
	a[1][3] = p3.abs2() - p1.abs2();
	rep(i, 3) a[2][i] = 2 * (p4.cor[i] - p1.cor[i]);
	a[2][3] = p4.abs2() - p1.abs2();
	gauss(3);
	poi o = poi(ans[0], ans[1], ans[2]);
	Float r2 = dis2(o, p1);
	return {o, r2};
}
void solve() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::shuffle(p + 1, p + 1 + n, std::default_random_engine(seed));
	Ball B = Ball2(p[1], p[2]);
	for (int i = 3; i <= n; ++i) {
		if (inBall(B, p[i])) continue;
		B = Ball2(p[1], p[i]);
		for (int j = 2; j < i; ++j) {
			if (inBall(B, p[j])) continue;
			B = Ball2(p[i], p[j]);
			for (int k = 1; k < j; ++k) {
				if (inBall(B, p[k])) continue;
				B = Ball3(p[i], p[j], p[k]);
				for (int u = 1; u < k; ++u) {
					if (inBall(B, p[u])) continue;
					B = Ball4(p[i], p[j], p[k], p[u]);
				}}}}
	double ans = std::sqrt(B.second);
	printf("%.3f\n", ans);
}
