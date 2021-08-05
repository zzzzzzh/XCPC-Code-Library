namespace sais {
	int t[max_N << 1], sa[max_N], rk[max_N], ht[max_N];
	int p[max_N], cnt[max_N], cur[max_N];
#define pS(x) sa[cur[s[x]]--] = x
#define pL(x) sa[cur[s[x]]++] = x
#define inducedSort(v) \
	std::fill_n(sa, n, -1), std::fill_n(cnt, m, 0);\
	for (int i = 0; i < n; i++) cnt[s[i]]++;\
	for (int i = 1; i < m; i++) cnt[i] += cnt[i-1];\
	for (int i = 0; i < m; i++) cur[i] = cnt[i]-1;\
	for (int i = n1 - 1; ~i; i--) pS(v[i]);\
	for (int i = 1; i < m; i++) cur[i] = cnt[i-1];\
	rep(i,0,n) if (sa[i]>0&&t[sa[i]-1]) pL(sa[i]-1);\
	for (int i = 0; i < m; i++) cur[i] = cnt[i]-1;\
	per(i,n-1,-1) if(sa[i]>0&&!t[sa[i]-1]) pS(sa[i]-1)
	void sais(int n, int m, int *s, int *t, int *p) {
		int n1 = t[n - 1] = 0, ch = rk[0] = -1, *s1 = s + n;
		per(i,n-2,-1) t[i]=s[i]==s[i+1]?t[i+1]:s[i]>s[i+1];
		rep(i,1,n) rk[i]=t[i-1]&&!t[i]?(p[n1] = i, n1++) : -1;
		inducedSort(p);
		for (int i = 0, x, y; i < n; i++)
			if (~(x = rk[sa[i]])) {
				if (ch < 1 || p[x+1]-p[x] != p[y+1]-p[y]) ch++;
				else for (int j=p[x],k=p[y]; j<=p[x+1]; j++,k++)
					if ((s[j] << 1 | t[j]) != (s[k] << 1|t[k])) {
						ch++;break;
					}
				s1[y = x] = ch;
			}
		if (ch + 1 < n1) sais(n1, ch + 1, s1, t + n, p + n1);
		else for (int i = 0; i < n1; i++) sa[s1[i]] = i;
		for (int i = 0; i < n1; i++) s1[i] = p[sa[i]];
		inducedSort(s1);
	}
	// ensure s[n] is the unique smallest
	void solve(int *s, int n, int m) { // s[max_N << 1]
		sais(++n, m, s, t, p);
		for (int i = 0; i < n; ++i) rk[sa[i]] = i;
		for (int i = 0, h = ht[0] = 0; i < n - 1; i++) {
			int j = sa[rk[i] - 1];
			while(i+h<n && j+h<n && s[i+h] == s[j+h]) ++h;
			if (ht[rk[i]] = h) --h;
		}
	}
}