namespace domi_tree {
	int n, m, s, Q[max_N], anc[max_N][log_N], dep[max_N], deg[max_N];
	std::vector<int> vec[max_N];
	inline void init_anc(int u) {
		dep[u] = dep[anc[u][0]] + 1;
		for (int i = 1; i < log_N; ++i)
			anc[u][i] = anc[anc[u][i - 1]][i - 1];
	}
	inline int lca(int u, int v) {
		if (dep[u] > dep[v]) std::swap(u, v);
		int k = dep[v] - dep[u];
		for (int i = 0; k; ++i, k >>= 1)
			if (k & 1) v = anc[v][i];
		if (u == v) return u;
		for (int i = log_N - 1; ~i; --i) {
			if (anc[u][i] == anc[v][i]) continue;
			u = anc[u][i]; v = anc[v][i];
		}
		return anc[u][0];
	}
	void solve(int s) {
		int tail = 0; Q[++tail] = s;
		for (int i = 1; i <= tail; ++i) {
			int u = Q[i]; init_anc(u);
			for (auto v : vec[u]) {
				if (!--deg[v]) Q[++tail] = v;
				if (anc[v][0]) anc[v][0] = lca(anc[v][0], u);
				else anc[v][0] = u;
			}}}}
