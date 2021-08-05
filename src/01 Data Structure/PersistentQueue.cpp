<TeX>树上 $dp$。从上到下 $w,v,u$，$u$ 的决策 $w$ 优于 $v$。$\frac{gety(w,v)}{getx(w,v)}\le d[u]$。$d$ 单调。二分 head 和 tail ，记录 tail 改变前的位置和值，回溯时撤销。</TeX>
ll dp[MAXN], d[MAXN];
int q[MAXN];
void dfs(int u, int fa, int head, int tail) {
	int l = head, r = tail - 2;
	while (l < r) {
		int m = (l + r) >> 1;
		if(gety(q[m+1],q[m])<=d[u]*getx(q[m+1],q[m]))
			l = m + 1;
		else r = m;
	}
	if(l+1<tail&&gety(q[l+1],q[l])<=d[u]*getx(q[l+1],q[l]))
		++l;
	head = l;
	dp[u] = dp[q[head]] + sqr(d[u] - d[q[head]]) + p;
	l = head + 2, r = tail;
	while (l < r) {
		int m = (l + r) >> 1;
		if (gety(q[m-1],q[m-2])*getx(u,q[m-1]) 
			  >=gety(u,q[m-1])*getx(q[m-1],q[m-2])) {
			r = m;
		} else l = m + 1;
	}
	if(l-1>head&&gety(q[l-1],q[l-2])*getx(u,q[l-1]) 
		  >=gety(u,q[l-1])*getx(q[l-1],q[l-2]))
		--l;
	tail = l;
	int sit = tail, value = q[tail];
	q[tail++] = u;
	for (auto &p : vec[u]) {
		int v = p.first, w = p.second;
		if (v == fa) continue;
		d[v] = d[u] + w; dfs(v, u, head, tail);
	}
	q[sit] = value;
}
void solve() {
	dp[1] = -p; d[1] = 0; q[0] = 1;
	for (auto &p : vec[1]) {
		int v = p.first, w = p.second;
		d[v] = d[1] + w; dfs(v, 1, 0, 1);
	}
}
