namespace maxFlow {
	void addEdge(int u, int v, flowType c) {
		e.push_back({v, head[u], c}); head[u] = M++;
	}}
namespace GHtree {
	int n, m, u[max_M], v[max_M], w[max_M]; // origin graph
	int tu[max_N], tv[max_N], tw[max_N], tot; // GH tree
	int p[max_N], q[max_N];
	void solve(int l, int r) {
		std::random_shuffle(p + l, p + r + 1);
		maxFlow::init(n + 2);
		int s = n + 1, t = n + 2;
		maxFlow::addEdge(s, p[l], maxFlow::inf);
		maxFlow::addEdge(p[l], s, 0);
		maxFlow::addEdge(p[r], t, maxFlow::inf);
		maxFlow::addEdge(t, p[r], 0);
		for (int i = 1; i <= m; ++i) {
			maxFlow::addEdge(u[i], v[i], w[i]);
			maxFlow::addEdge(v[i], u[i], w[i]);
		}
		tw[++tot] = maxFlow::dinic(n + 2, s, t);
		tu[tot] = p[l], tv[tot] = p[r];
		int L = l, R = r;
		for (int i = l; i <= r; ++i) {
			int x = p[i];
			if (~maxFlow::dis[x]) q[L++] = x;
			else q[R--] = x;
		}
		for (int i = l; i <= r; ++i) p[i] = q[i];
		solve(l, L - 1), solve(L, r);
	}
	void testCase() { //input
		tot = 0;
		for (int i = 1; i <= n; ++i) p[i] = i;
		solve(1, n);
	}}
