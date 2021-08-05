namespace SAM {
	int tot, root, last, first_endpos[MAXN << 1];
	int go[MAXN<<1][26], par[MAXN<<1], maxlen[MAXN<<1];
	ll same[MAXN << 1];
	void init() { root = last = newnode(tot = 0); }
	int newnode(int ml) {
		maxlen[++tot] = ml;
		memset(go[tot], 0, sizeof go[tot]);
		par[tot] = 0; same[tot] = 1;
		return tot;
	}
	void extend(int w, int pos) { //w = ch - 'a'
		int p = last, np = newnode(maxlen[p] + 1);
		first_endpos[np] = pos;
		for (; p && !go[p][w]; p = par[p]) go[p][w] = np;
		if (!p) par[np] = root; 
		else {
			int q = go[p][w];
			if (maxlen[p] + 1 == maxlen[q]) par[np] = q; 
			else {
				int nq = newnode(maxlen[p] + 1);
				memcpy(go[nq], go[q], sizeof go[q]);
				same[nq] = 0;//
				first_endpos[nq] = first_endpos[q];
				par[nq] = par[q];
				par[np] = par[q] = nq;
				for (; p&&go[p][w]==q; p=par[p]) go[p][w] = nq;
			}
		}
		last = np;
	}
	void build(char *s) {
		for (int i = 0; s[i]; ++i) extend(s[i] - 'a', i + 1);
	}
	int cnt[MAXN], ord[MAXN << 1];
	void sort() {
		int n = maxlen[last];
		for (int i = 0; i <= n; ++i) cnt[i] = 0;
		for (int i = 1; i <= tot; ++i) cnt[maxlen[i]]++;
		for (int i = 1; i <= n; ++i) cnt[i] += cnt[i - 1];
		for (int i = tot; i; --i) ord[cnt[maxlen[i]]--] = i;
	}
}
