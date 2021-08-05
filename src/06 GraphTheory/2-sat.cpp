struct twoSat{   //for 2-sat MAXN is double large
	int col[MAXN], ncnt, scnt;
	std::vector<int> vec[MAXN << 1];
	int dfn[MAXN << 1], low[MAXN << 1], bel[MAXN << 1];
	std::stack<int> stack;
	void tarjan(int u){
		dfn[u] = low[u] = ++ncnt;
		stack.push(u);
		for(auto v : vec[u]){
			if(!dfn[v]){
				tarjan(v); low[u] = std::min(low[u], low[v]);
			} else if(!bel[v]) low[u] = std::min(low[u],dfn[v]);
		}
		if (low[u] != dfn[u]) return;
		int v; ++scnt;
		do {
			v = stack.top(); stack.pop(); bel[v] = scnt;
		} while(v != u);
	}
	bool solve(){ //tarjan + topsort
		memset(dfn, 0, sizeof(dfn));
		ncnt = scnt = 0;
		for(int i = 1; i <= n + n; ++ i)
			if(!dfn[i]) tarjan(i);
		for(int i = 1; i <= n; ++ i) {
			if(bel[i] == bel[i + n]) return false;
			col[i] = (bel[i] < bel[i + n]);
		}
		return true;
	}
}
