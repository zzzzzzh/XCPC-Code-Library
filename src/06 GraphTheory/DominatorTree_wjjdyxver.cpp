namespace domi_tree {
	int n, m, par[max_N], vp[max_N], dfn[max_N], cnt, sdom[max_N], idom[max_N];
	inline bool cmp(int u, int v) {return dfn[u] < dfn[v];}
	std::vector<int> vec[max_N], _vec[max_N], tmp[max_N];
	int f[max_N], pos[max_N];
	int find(int u) {
		if (f[u] == u) return u;
		int v = f[u]; f[u] = find(v);
		if (cmp(sdom[pos[v]], sdom[pos[u]])) pos[u] = pos[v];
		return f[u];
	}
	void dfs(int u) {
		vp[dfn[u] = ++cnt] = u;
		for (auto v : vec[u]) {
			if (dfn[v]) continue;
			par[v] = u; dfs(v);
		}
	}
	void solve(int rt) {
		for (int i = 1; i <= n; ++i) {
			par[i] = dfn[i] = 0;
			f[i] = pos[i] = sdom[i] = i;
		}
		dfs(rt);
		for (int i = cnt; i > 1; --i) {
			int u = vp[i];
			for (auto v : _vec[u]) {
				if (!dfn[v]) continue;
				find(v);
				if (cmp(sdom[pos[v]], sdom[u]))
					sdom[u] = sdom[pos[v]];
			}
			tmp[sdom[u]].push_back(u);
			int p = f[u] = par[u];
			for (auto v : tmp[p]) {
				find(v);
				idom[v] = (sdom[v] == sdom[pos[v]]) ? p : pos[v];
			}
			std::vector<int>().swap(tmp[p]);
		}
		for (int i = 2; i <= cnt; ++i) {
			int u = vp[i];
			if (idom[u] != sdom[u]) idom[u] = idom[idom[u]];
		}
	}
	void clear() {
		cnt = 0;
		for (int i = 1; i <= n; ++i) {
			std::vector<int>().swap(vec[i]);
			std::vector<int>().swap(_vec[i]);
		}}}
