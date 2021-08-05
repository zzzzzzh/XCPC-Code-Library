<TeX>最大团，复杂度上限大概是 $ O(3^{\frac{n}{3}}) $ 左右。G是邻接矩阵，$dp_i$表示由$i$到$n-1$的子图构成的最大团点数，剪枝用。adj中存放的是与i邻接且标号比i大的顶点。顺便可以保证方案的字典序最小。另外对adj进行压位有非常良好的效果。</TeX>
struct MaxClique{
	bool g[MAXN][MAXN];
	int n, nmax[MAXN], alt[MAXN][MAXN], ans;
	void init(int n){
		this -> n = n;
		memset(g, 0, sizeof(g));
	}
	bool dfs(int cur, int tot){
		if(!cur){
			if(ans < tot){ ans = tot; return 1; }
			return false;
		}
		for(int i = 0; i < cur; ++ i){
			if(tot + (cur - i) <= ans) return 0;
			int u = alt[tot][i];
			if(tot + nmax[u] <= ans) return 0;
			int nxt = 0;
			for(int j = i + 1; j < cur; ++ j)
				if(g[u][alt[tot][j]])
					alt[tot + 1][nxt ++] = alt[tot][j];
			if(dfs(nxt, tot + 1)) return 1;
		}
		return 0;
	}
	int solve(){
		ans = 0;
		memset(nmax, 0, sizeof(nmax));
		for(int i = n; i; -- i){
			int cur = 0;
			for(int j = i + 1; j <= n; ++ j)
				if(g[i][j]) alt[1][cur ++] = j;
			dfs(cur, 1); nmax[i] = ans;
		}
		return ans;
	}
};
