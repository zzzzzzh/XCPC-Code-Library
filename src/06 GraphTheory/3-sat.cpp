<TeX>ans init $-1$, found init false. $x$ and $\lnot x$ should be re-indexed to $2x+1$ and $2x$. $\bigwedge_{u=0}^{C-1}(W[0][u] \vee W[1][u] \vee W[2][u])$</TeX>
int C, W[3][MAXN], ans[26];
bool found;
bool sat(int h) { return ans[h >> 1] == (h & 1); }
void dfs(int u) {
	while(u<C&&(sat(W[0][u])|sat(W[1][u])|sat(W[2][u])))++u;
	if (u == C) { found = true; return; }
	int tmp[3];
	for (int i = 0; i < 3; ++i) tmp[i] = ans[W[i][u]>>1];
	for (int i = 0; i < 3; ++i)
		if (ans[W[i][u] >> 1] == -1) {
			ans[W[i][u] >> 1] = W[i][u] & 1;
			dfs(u + 1); if (found) return;
			ans[W[i][u] >> 1] = 1 - ans[W[i][u] >> 1];
		} else if (sat(W[i][u])) {
			dfs(u + 1); if (found) return;
		}
	for(int i = 0; i < 3; ++i) ans[W[i][u]>>1] = tmp[i];
}
