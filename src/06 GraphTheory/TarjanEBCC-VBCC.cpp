<TeX> Tarjan EBCC and VBCC：每个 VBCC 新建点 tot , tot 的父亲为 VBCC 中深度最小的点 u , 其余点 v 的父亲为 tot。原图中每个点的信息维护在树中对应点和其父亲节点处, 若路径 lca 为新建点, 其父亲的信息也要考虑 </TeX>
namespace Tarjan {
	int dfn[max_N], low[max_N], cnt;
	int cut[max_N], par[max_N * 2];
	int st[max_N], top, tot;
	int ebccno[max_N], ebcc_cnt;
	vector<int> vec[max_N], ebcc[max_N], bcc[max_N * 2];
	void tarjan(int u, int p) {
		dfn[u] = low[u] = ++cnt;
		st[++top] = u;
		for (auto &v : vec[u]) {
			if (v == p) continue;
			if (!dfn[v]) {
				tarjan(v, u);
				low[u] = std::min(low[u], low[v]);
				//bridge(u,v)=dfn[u]<low[v]&&!multiEdge(u,v);
				if (dfn[u] <= low[v]) {
					cut[u] = 1;
					int x = -1;
					par[++tot] = u;
					bcc[u].push_back(tot);
					while (x != v) {
						x = st[top--];
						bcc[tot].push_back(x);
						par[x] = tot;
					}
				}
			} else if (dfn[v] < dfn[u])
				low[u] = std::min(low[u], dfn[v]);
		}
		if (!p && vec[u].size() <= 1) cut[u] = 0;
		/*if (dfn[u] == low[u]) {
			++ebcc_cnt;
			int v = -1;
			while (u != v) {
				ebccno[v = st[top--]] = ebcc_cnt;
				ebcc[ebcc_cnt].push_back(v);
			}
		}*/
	}
	void solve(int n) {
		tot = n;
		for (int i = 1; i <= n; ++i) if(!dfn[i]) tarjan(i,i);
	}
}
