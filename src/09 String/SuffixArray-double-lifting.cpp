namespace SuffixArray { // 字符串从1开始
	int sa[max_N], rk[max_N], height[max_N];
	int tmp[max_N], cnt[max_N], bc[max_N];
	inline void build(const char *s, int n) {
		int i, j, k, T, sigma;
		for (i = 1; i <= n; ++i) sa[i] = i;
		sort(sa+1,sa+1+n,[&](int a,int b){return s[a]<s[b];});
		for (i = 2, bc[sa[1]] = 1; i <= n; ++i)
			bc[sa[i]] = bc[sa[i-1]] + (s[sa[i-1]] < s[sa[i]]);
		for (T = 1; (sigma = bc[sa[n]]) < n; T <<= 1) {
			auto cmp = [&](const int &a, const int &b) {
				if (bc[a] != bc[b]) return bc[a] < bc[b];
				return (a+T>n||b+T>n)?a>b:bc[a+T] < bc[b+T];
			};
			for (i = 0; i <= sigma; ++i) cnt[i] = 0;
			for (i = 1; i <= n; ++i) ++cnt[(i+T>n)?0:bc[i+T]];
			for (i = 1; i <= sigma; ++i) cnt[i] += cnt[i - 1];
			for (i = n; i; --i)
				tmp[cnt[(sa[i]+T>n)?0:bc[sa[i]+T]]--] = sa[i];
			for (i = 0; i <= sigma; ++i) cnt[i] = 0;
			for (i = 1; i <= n; ++i) ++cnt[bc[i]];
			for (i = 1; i <= sigma; ++i) cnt[i] += cnt[i - 1];
			for (i = n; i; --i) sa[cnt[bc[tmp[i]]]--] = tmp[i];
			for (i = 2, tmp[sa[1]] = 1; i <= n; ++i)
				tmp[sa[i]] = tmp[sa[i-1]] + cmp(sa[i-1], sa[i]);
			memcpy(bc + 1, tmp + 1, sizeof(int) * n);
		}
		for (i = 1; i <= n; ++i) rk[sa[i]] = i;
		for (i = 1, k = 0; i <= n; ++i) {
			if (rk[i] == 1) k = 0;
			else {
				j = sa[rk[i] - 1]; if (k) --k;
				while(i+k<=n && j+k<=n && s[i+k]==s[j+k]) ++k;
			}
			height[rk[i]] = k;
		}}}
