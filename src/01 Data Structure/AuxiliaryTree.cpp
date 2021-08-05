<TeX>Build auxiliary tree for key vertices st[1...top].$O(n\log{n})$</TeX>
namespace mini_tree {
	int st[max_N << 1], top;
	struct functor {
		bool operator()(int x, int y) const {
			return dfnL[x] < dfnL[y];
		}
	};
	inline bool isanc(int u, int v) {
		return dfnL[u] <= dfnL[v] && dfnL[v] <= dfnR[u];
	}
	std::vector<std::pair<int, int>> _vec[max_N];
	void build() {
		std::sort(st + 1, st + 1 + top, functor());
		int tmp = top;
		rep (i, 1, tmp) st[++top] = lca(st[i], st[i + 1]);
		std::sort(st + 1, st + 1 + top, functor());
		tmp = std::unique(st + 1, st + 1 + top) - (st + 1);
		top = 1;
		for (int i = 2; i <= tmp; ++i) {
			while (top && !isanc(st[top], st[i])) --top;
			_vec[st[top]].e_b(st[i],dep[st[i]]-dep[st[top]]);
			st[++top] = st[i];
		}}}
