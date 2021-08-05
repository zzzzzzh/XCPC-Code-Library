<TeX>字符串从 $0$ 开始</TeX>
namespace minimal_representation { 
	char s[max_N << 1];
	int solve(char *S, int n) {
		memcpy(s, S, sizeof(char) * n);
		memcpy(s + n, S, sizeof(char) * n);
		int i = 0, j = 1, k = 0;
		while (i < n && j < n && k < n) {
			int t = s[i + k] - s[j + k];
			if (!t) ++k;
			else {
				if (t < 0) j += k + 1; else i += k + 1;
				if (i == j) ++j;
				k = 0;
			}
		}
		return std::min(i, j);
	}
}