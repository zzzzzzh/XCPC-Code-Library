namespace maxFlow {
	using flowType = long long;
	const flowType inf = 0x3f3f3f3f3f3f3f3fll;
	struct edge {
		int v, n; flowType c; // next,capacity
	};
	std::vector<edge> e; 
	int M, head[max_N], cur[max_N], dis[max_N];
	bool bfs(int n, int s, int t) {
		static int Q[max_N];
		int tail = 0;
		memset(dis + 1, -1, n * sizeof(int));
		dis[s] = 0, Q[tail++] = s;
		for (int i = 0; i < tail; ++i) {
			int u = Q[i];
			for (int it = head[u]; ~it; it = e[it].n) {
				if (!e[it].c) continue;
				int v = e[it].v;
				if (dis[v] == -1) {
					dis[v] = dis[u] + 1; Q[tail++] = v;
				}}}
		return dis[t] != -1;
	}
	flowType dfs(int t, int u, flowType a) {
		if (u == t) return a;
		flowType ret = 0, tmp;
		for (int &it = cur[u]; ~it; it = e[it].n) {
			int v = e[it].v;
			if (!e[it].c || dis[u] + 1 != dis[v]) continue;
			tmp = dfs(t, v, std::min(a - ret, e[it].c));
			if (tmp) {
				e[it].c -= tmp, e[it ^ 1].c += tmp;
				if ((ret += tmp) == a) break;
			}
		}
		if (!ret) dis[u] = -1;
		return ret;
	}
	void init(int n) {
		e.clear(); M = 0;
		memset(head + 1, -1, n * sizeof(int));
	}
	void addEdge(int u, int v, flowType c) {
		e.push_back({v, head[u], c}); head[u] = M++;
		e.push_back({u, head[v], 0}); head[v] = M++;
	}
	flowType dinic(int n, int s, int t) {
		flowType ret = 0;
		while (bfs(n, s, t)) {
			memcpy(cur + 1, head + 1, n * sizeof(int));
			ret += dfs(t, s, inf);
		}
		return ret;
	}
}

