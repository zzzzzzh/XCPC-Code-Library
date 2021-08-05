<TeX>有撤销操作时，势能分析失效，需要预处理转移。时空复杂度 $O(n\sigma)$</TeX>
namespace KMP {
	int fail[max_N], trans[max_N][sigma],last[max_N][sigma];
	char s[max_N];
	void init(int n) {
		fail[1] = 0;
		for (int i = 2; i <= n; ++i) {
			int &j = fail[i] = fail[i - 1];
			for (; j && s[i] != s[j + 1]; j = fail[j]);
			if (s[i] == s[j + 1]) ++j;
		}
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < sigma; ++j)
				last[i][j] = last[fail[i]][j];
			last[i][s[i + 1] - 'a'] = i;
			for (int j = 0; j < sigma; ++j) {
				trans[i][j] = last[i][j];
				if (s[trans[i][j] + 1] - 'a' == j) ++trans[i][j];
			}
		}
		for (int j = 0; j < sigma; ++j)
			trans[n][j] = trans[fail[n]][j];
	}
}
