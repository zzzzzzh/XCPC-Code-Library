<TeX>稠密图 kosaraju 求 scc。$O(\frac{n^2}{64})$</TeX>
struct BIT{
	ui v[8];
	void clear(){ for(int i = 0; i < 8; ++ i) v[i] = 0; }
	void set(int x){       v[x >> 5] |= 1U << (x & 31); }
	void flip(int x){      v[x >> 5] ^= 1U << (x & 31); }
	int get(int x){ return v[x >> 5] >> (x & 31) & 1; }
}vis, gra[MAXN], arg[MAXN];
std::stack<int> stack;
void sfd(int u){
	vis.flip(u);
	for(int i = 0; i < 8; ++ i) while(1){
		ui o = vis.v[i] & arg[u].v[i]; if(!o) break;
		sfd(i << 5 | __builtin_ctz(o));
	}
	stack.push(u);
}
void dfs(int u){
	vis.flip(u); ++now;
	for(int i = 0; i < 8; ++ i) while(1){
		ui o = vis.v[i] & gra[u].v[i]; if(!o) break;
		dfs(i << 5 | __builtin_ctz(o));
	}
}
void solve(){
	rep(i, 0, n) gra[i].clear(), arg[i].clear();
	// edge i->j gra[i].set(j); arg[j].set(i);
	for(int i = 0; i < n; ++i) vis.set(i);
	for(int i = 0; i < n; ++i) if(vis.get(i)) sfd(i);
	for(int i = 0; i < n; ++i) vis.set(i);
	while (!stack.empty()) {
		int i = stack.top(); stack.pop();
		if(vis.get(i)) { now = 0; dfs(i); }
	}
}
