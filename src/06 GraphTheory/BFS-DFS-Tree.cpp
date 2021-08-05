int dfn_l[MAXN], dfn_r[MAXN], f[MAXN][21];
int nxt[MAXN], dep[MAXN], sz[MAXN];
std::vector<int> vec[MAXN];
void bfs(int u){
	std::vector<int> q;
	q.push_back(u);
	int tot = 1;
	for(int i = 0; i < tot; ++ i){
		u = q[i]; sz[u] = 1;
		for(auto v : vec[u]){
			if(v == f[u][0]) continue;
			dep[v] = dep[u] + 1; f[v][0] = u;
			for(int j = 0; f[v][j + 1] = f[f[v][j]][j]; ++ j);
			q.push_back(v); ++tot;
		}
	}
	for(int i=tot-1; ~i; --i) sz[f[q[i]][0]]+=sz[q[i]];
	dfn_l[1] = 1; nxt[1] = 2;
	int last;
	for(int i = 1; i < tot; ++ i){
		if(f[q[i]][0]!=f[q[i-1]][0]) last=nxt[f[q[i]][0]];
		else last += sz[q[i - 1]];
		dfn_l[q[i]] = last; nxt[q[i]] = last + 1;
	}
	for(int i=1; i<=n; ++i) dfn_r[i]=dfn_l[i]+sz[i]-1;
}
