int seq[MAXN*2], logdown[MAXN*2], st[MAXN*2][MAXB], cnt;
int dep[MAXN], pos[MAXN];
void dfs(int u, int fa){
	seq[++cnt] = u; pos[u] = cnt;
	for(auto v : vec[u]){
		if(v == fa) continue;
		dep[v] = dep[u] + 1;
		dfs(v, u); seq[++cnt] = u;
	}
}
void make_st_table(){
	for(int i=2; i<=cnt; ++i) logdown[i]=logdown[i>>1]+1;
	for(int i = 1; i <= cnt; ++i) st[i][0] = i;
#define depless(u, v) (dep[seq[u]] < dep[seq[v]] ? u : v)
	for(int d = 1; d <= logdown[cnt]; ++ d)
		for(int i = 1; i + (1 << d - 1) <= cnt; ++ i)
			st[i][d]=depless(st[i][d-1],st[i+(1<<d-1)][d-1]);
}
int lca(int u, int v){
	if(!u || !v) return u + v;
	u = pos[u]; v = pos[v];
	if(u > v) std::swap(u, v);
	int d = logdown[v - u + 1];
	return seq[depless(st[u][d], st[v - (1 << d) + 1][d])];
}
