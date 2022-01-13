/*
 * @param 自己在 vec 中加边
 * @return scc_cnt 是 SCC 数量，sccno 表示每个点的 SCC 编号（从 1 开始），scc 存储了每个 SCC 的元素
 */
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
