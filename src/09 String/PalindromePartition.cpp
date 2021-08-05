<TeX>Given a string s, find the number of ways to split s to substrings such that if there are k substrings (p1, p2, ..., pk) in partition, then $p_i = p_{k-i+1}$ for all $i \in [1, k]$ and $k$ is even. $\text{series_dp}[v] = \sum_{j \in \text{series}(v)} dp[i - len[j]]$ </TeX>
PalindromicTree pt;
int series_dp[max_N], dp[max_N];
int main() {
	pt.init();
	scanf("%s", b + 1); n = strlen(b + 1);
	for (int i = 1; i + i <= n + 1; ++i) {
		a[i + i - 1] = b[i]; a[i + i] = b[n - i + 1];
	}
	a[0] = -1;
	memset(dp + 1, 0, n * sizeof(int));
	dp[0] = 1;
	for (int i = 1; i <= n; ++i) {
		pt.append(i, a[i] - 'a'); dp[i] = 0;
		for(int v = pt.last; pt.len[v] > 0; v = pt.slink[v]) {
			series_dp[v] = dp[i - (pt.len[pt.slink[v]] + pt.diff[v])];
			if (pt.diff[v] == pt.diff[pt.link[v]])
				(series_dp[v] += series_dp[pt.link[v]]) >= mod && (series_dp[v] -= mod);
			if (i & 1) continue;
			(dp[i] += series_dp[v]) >= mod && (dp[i] -= mod);
		}
	}
	printf("%d\n", dp[n]);
	return 0;
}
