const int inf = 0x3f3f3f3f;
int n, m, dp[1 << size_m][size_n], pos[size_m];
void solve() {
	memset(dp, 0x3f, sizeof(dp));
	for (int i = 1; i <= n; ++i) dp[0][i] = 1;
	for (int i = 0; i < m; ++i) dp[1 << i][pos[i]] = 1; 
	for (int s = 1; s < (1 << m); ++s) {
		for (int u = 1; u <= n; ++u) {
			for (int s1 = (s - 1) & s; s1; (--s1) &= s)
				dp[s][u] = std::min(dp[s][u],
					dp[s1][u] + dp[s ^ s1][u] - 1);
			if (dp[s][u] < inf) pq.emplace(-dp[s][u], u);
		}
		dijkstra(s);
	}
}
