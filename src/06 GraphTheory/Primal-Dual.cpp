<TeX>Min cost feasible flow or max flow. modified to long long: inf, dis[], sum, cost, memset(dis, inf)*2, edge.w, addedge(w)</TeX>
namespace primal_dual {
	const int inf = 0x3f3f3f3f; // larger than all dis
	int M, dis[max_N], vis[max_N], sum, flow, cost;
	struct edge {
		int v, c, w; //vertex, capacity, weight
	};
	std::vector<edge> e; //all edges
	std::vector<int> vec[max_N]; // edge indexes
	void adjust(int s) {
		for (int it = 0; it < M; it += 2) {
			int u = e[it ^ 1].v, v = e[it].v;
			e[it].w += dis[v] - dis[u];
			e[it ^ 1].w += dis[u] - dis[v];
		}
		sum += dis[s];
	}
	void spfa(int n, int s, int t) {
		static int Q[max_N];
		int head = 0, tail = 0;
		memset(dis + 1, 0x3f, n * sizeof(int));
		memset(vis + 1, 0, n * sizeof(int));
		dis[t] = 0, vis[t] = 1, Q[tail++] = t;
		while (head != tail) {
			int u = Q[head++]; vis[u] = 0;
			if (head == max_N) head = 0;
			for (auto it : vec[u]) {
				if (!e[it ^ 1].c) continue;
				int v = e[it].v;
				auto tmp = dis[u] + e[it ^ 1].w;
				if (dis[v] > tmp) {
					dis[v] = tmp;
					if (!vis[v]) {
						vis[v] = 1, Q[tail++] = v;
						if (tail == max_N) tail = 0;
					}}}}
		if (dis[s] == inf) return;
		adjust(s);
	}
	bool dijkstra(int n, int s, int t) {
		std::priority_queue<std::pair<int, int>> pq;
		memset(dis + 1, 0x3f, n * sizeof(int));
		memset(vis + 1, 0, n * sizeof(int));
		dis[t] = 0, pq.push({0, t});
		while (!pq.empty()) {
			int u = pq.top().second; pq.pop();
			if (vis[u]) continue;
			vis[u] = 1;
			for (auto it : vec[u]) {
				if (!e[it ^ 1].c) continue;
				int v = e[it].v;
				auto tmp = dis[u] + e[it ^ 1].w;
				if (dis[v] > tmp) {
					dis[v] = tmp; pq.push({-dis[v], v});
				}
			}
		}
		if (!vis[s]) return false;
		return adjust(s), true;
	}
	int dfs(int t, int u, int a) {
		if (u == t) return a;
		vis[u] = 1;
		int ret = 0, tmp;
		for (auto it : vec[u]) {
			int v = e[it].v;
			if (e[it].w || vis[v] || !e[it].c) continue;
			tmp = dfs(t, v, std::min(a - ret, e[it].c));
			if (tmp) {
				e[it].c -= tmp;
				e[it ^ 1].c += tmp;
				if ((ret += tmp) == a) break;
			}
		}
		return ret;
	}
	void addEdge(int u, int v, int c, int w) {
		e.push_back({v, c, w}); vec[u].push_back(M++);
		e.push_back({u, 0, -w}); vec[v].push_back(M++);
	}
	void minCost(int n, int s, int t) {
		int a = ~0U >> 1; spfa(n, s, t);
		do {
			while (a) {
				memset(vis + 1, 0, n * sizeof(int));
				int adt = dfs(t, s, a);
				if (!adt) break;
				a -= adt; flow += adt; cost += adt * sum;
			}
		} while (dijkstra(n, s, t) && sum < 0); //feasible
		//} while (dijkstra(n, s, t)); //max flow
	}
	void clear(int n) {
		e.clear();
		for (int i = 1; i <= n; ++i)
			std::vector<int>().swap(vec[i]);
		M = sum = flow = cost = 0;
	}
}
