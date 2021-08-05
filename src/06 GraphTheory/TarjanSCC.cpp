namespace SCC {
	const int max_N = (int) 1e5 + 21;

	int n, m, dfn[max_N], low[max_N], cnt;

	int sccno[max_N], scc_cnt, st[max_N], top;

	std::vector<int> vec[max_N], scc[max_N];
	
	void dfs(int u) {
		st[++top] = u;
		dfn[u] = low[u] = ++cnt;

		for (auto v : vec[u]) {
			if (!dfn[v]) {
				dfs(v);
				low[u] = std::min(low[u], low[v]);
			} else if (!sccno[v]) {
				low[u] = std::min(low[u], dfn[v]);
			}
		}

		if (dfn[u] != low[u]) return;

		sccno[u] = ++scc_cnt;
		while (st[top] != u) {
			scc[scc_cnt].push_back(st[top]);
			sccno[st[top--]] = scc_cnt;
		}
		scc[scc_cnt].push_back(u);
		--top;
	}

	void solve(int n) {
		for (int i = 1; i <= n; ++i) if (!dfn[i]) dfs(i);
	}
};
