namespace EBCC {
    int ebcc_cnt, ebcc_no[max_N], st[max_N], top;
    int dfn[max_N], low[max_N], cnt;
    std::vector<int> vec[max_N], ebcc[max_N];
    void tarjan(int u, int p) {
        st[++top] = u;
        dfn[u] = low[u] = ++cnt;
        for (auto v : vec[u]) {
            if (v == p) continue;
            if (!dfn[v]) {
                tarjan(v, u);
                low[u] = std::min(low[u], low[v]);
				/*
				if (dfn[u] < low[v]) {
					// 若不存在重边则 u->v 是桥边
				}
				*/
            } else if (dfn[v] < dfn[u]) {
                low[u] = std::min(low[u], dfn[v]);
            }
        }
        if (dfn[u] == low[u]) {
            std::vector<int>().swap(ebcc[++ebcc_cnt]);
            do {
                ebcc_no[st[top]] = ebcc_cnt;
                ebcc[ebcc_cnt].push_back(st[top]);
            } while (st[top--] != u);
        }
    }
};