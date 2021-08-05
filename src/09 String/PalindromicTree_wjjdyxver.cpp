char a[max_N];
struct PalindromicTree {
	int tot, last, ch[max_N][sigma];
	int len[max_N], cnt[max_N], link[max_N];
	int diff[max_N], slink[max_N]; // palindrome series
	inline int new_node(int l) {
		int x = tot++; len[x] = l;
		std::fill_n(ch[x], sigma, 0);
		cnt[x] = link[x] = diff[x] = slink[x] = 0;
		return x;
	}
	inline int get_node(int n, int x) {
		while (a[n - len[x] - 1] != a[n]) x = link[x];
		return x;
	}
	// Ensure that s[0] is the unique character in str.
	inline void append(int n, int c) {
		int u = get_node(n, last);
		int v = ch[u][c];
		if (!v) {
			v = new_node(len[u] + 2);
			link[v] = ch[get_node(n, link[u])][c];
			ch[u][c] = v;
			diff[v] = len[v] - len[link[v]];
			if (diff[v]==diff[link[v]]) slink[v]=slink[link[v]];
			else slink[v] = link[v];
		}
		++cnt[last = v];
	}
	inline void get_cnt() {
		for (int i = tot - 1; ~i; --i) cnt[link[i]] += cnt[i];
	}
	inline void init() {
		last = tot = 0;
		int rt = new_node(0);
		link[rt] = new_node(-1);
	}
};
