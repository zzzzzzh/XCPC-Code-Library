<TeX>求字符串 $s$ 的每个后缀和 $t$ 的 lcp 长度, 思路类似 manacher. $O(n+m)$</TeX>
namespace exKMP {
	int nxt[max_N], ext[max_N];
	// nxt[i] : t[i...m] 与 t 的 lcp 长度
	// ext[i] : s[i...n] 与 t 的 lcp 长度
	void solve(const char *s, int n, const char *t, int m) { 
		nxt[1] = m;
		for (int &x=nxt[2]=0; x<m-1 && t[x+1] == t[x+2]; ++x);
		for (int a = 2, i = 3; i <= m; ++i) {
			int p = a + nxt[a] - 1, q = nxt[i - a + 1];
			if (i + q <= p) nxt[i] = q;
			else {
				int &j = nxt[i] = std::max(0, p - i + 1);
				while (i + j <= m && t[i + j] == t[j + 1]) ++j;
				a = i;
			}
		}
		for(int &x=ext[1]=0; x<n&&x<m&&s[x+1]==t[x+1]; ++x);
		for (int a = 1, i = 2; i <= n; ++i) {
			int p = a + ext[a] - 1, q = nxt[i - a + 1];
			if (i + q <= p) ext[i] = q;
			else {
				int &j = ext[i] = std::max(0, p - i + 1);
				while(i+j<=n && j<=m && s[i+j]==t[j+1]) ++j;
				a = i;
			}}}}