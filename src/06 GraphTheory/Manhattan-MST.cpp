<TeX>最小生成树。若最大生成树，找到 $x+y,x-y$ 最大/小的点 $A,B,C,D$，每个点向这四个点连边，权值为曼哈顿距离。</TeX>
int n;
struct P{
	int x, y, id;
	bool operator < (const P &rhs) const {
		return x != rhs.x ? x < rhs.x : y < rhs.y;
	}
}p[MAXN];
struct MST{
	int tot, fa[MAXN], a[MAXN], b[MAXN];
	std::pair<int, int> bit[MAXN];
	struct Edge{
		int u, v, w;
		bool operator < (const Edge &rhs) const {
			return w < rhs.w;
		}
	}edge[MAXN << 2];
	int find(int x){ return x == fa[x] ? x : (fa[x] = find(fa[x])); }
	int dis(P a, P b){ return std::abs(a.x - b.x) + std::abs(a.y - b.y); }
	void insert(int x, int val, int pos){
		for(int i = x; i; i -= lowbit(i))
			if(val < bit[i].first) bit[i] = {val, pos};
	}
	int query(int x, int lmt){
		int ret1 = INT_MAX, ret2 = -1;
		for(int i = x; i <= lmt; i += lowbit(i))
			if(ret1 > bit[i].first){
				ret1 = bit[i].first;
				ret2 = bit[i].second;
			}
		return ret2;
	}
	int solve(){ // init
		tot = 0;
		for(int dir = 0; dir <= 3; ++ dir){
			if(dir&1)for(int i=1;i<=n;++i) swap(p[i].x, p[i].y);
			else if(dir) for(int i=1; i<=n; ++i) p[i].x=-p[i].x;
			std::sort(p + 1, n + p + 1);
			for(int i = 1; i <= n; ++i) a[i]=b[i]=p[i].y-p[i].x;
			std::sort(b + 1, n + b + 1);
			int lmt = std::unique(b + 1, n + b + 1) - b - 1;
			for(int i = 1; i <= lmt; ++i) bit[i]={INT_MAX, -1};
			for(int i = n; i; -- i){
				int pos = lower_bound(b+1, lmt+b+1, a[i]) - b;
				int ans = query(pos, lmt);
				if(ans != -1){
					edge[++tot].u = p[i].id;
					edge[tot].v = p[ans].id;
					edge[tot].w = dis(p[i], p[ans]);
				}
				insert(pos, p[i].x + p[i].y, i);
			}
		}
		std::sort(edge + 1, tot + edge + 1);
		for(int i = 1; i <= n; ++ i) fa[i] = i;
		int cnt = 0, ret = 0;
		for(int i = 1; i <= tot; ++ i){
			int u = find(edge[i].u), v = find(edge[i].v);
			if(u == v) continue;
			fa[u] = v;
			++ cnt; ret += edge[i].w;
			if(cnt == n - 1) break;
		}
		return ret;
	}
} mst;
