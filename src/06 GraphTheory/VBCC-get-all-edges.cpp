<TeX> 有时候需要找出点双中的所有边, 可以使用下面的写法, 增加维护边的栈, 下面为求所有恰好只出现在一个简单环中的所有边的代码 </TeX>
namespace Tarjan {
	int dfn[max_N], low[max_N], cnt;
	int stV[max_N], topV, stE[max_M], topE, ans;
	std::vector<std::pair<int, int>> vec[max_N];
	// first : v, second : id
	bool ret[max_M];
	void tarjan(int u, int pre) {
		dfn[u] = low[u] = ++cnt;
		stV[++topV] = u;
		for (auto e : vec[u]) {
			int v = e.first, w = e.second;
			if (w == pre) continue;
			if (!dfn[v]) {
				stE[++topE] = w; tarjan(v, w);
				low[u] = std::min(low[u], low[v]);
				if (dfn[u] <= low[v]) {
					int x = -1, sizV = 1;
					while (x != v) {
						x = stV[topV--];
						++sizV;
					}
					std::vector<int> tmp;
					int y = -1, sizE = 0;
					while (y != w) {
						tmp.push_back(y = stE[topE--]);
						++sizE;
					}
					if (sizV == sizE) 
						for (auto i : tmp) {
							++ans;
							ret[i] = true;
						}
				}
			} else if (dfn[v] < dfn[u]) {
				stE[++topE] = w;
				low[u] = std::min(low[u], dfn[v]);
			}}}}
