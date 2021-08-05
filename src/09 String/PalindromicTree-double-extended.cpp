struct PalindromicTree {
	int tot, first, last, ch[max_N][sigma];
	int len[max_N],link[max_N],link_len[max_N];
	inline int new_node(int L) {
		int x = tot++; len[x] = L;
		memset(ch[x], 0, sizeof(ch[x]));
		link[x] = link_len[x] = 0;
		return x;
	}
	inline void addLast(int st, int ed, int c) {
		auto get_node = [&](int x) {
			while (ed-len[x]-1<st || s[ed-len[x]-1] != s[ed])
				x = link[x];
			return x;
		};
		int u = get_node(last), v = ch[u][c];
		if (!v) {
			v = new_node(len[u] + 2);
			link[v] = ch[get_node(link[u])][c];
			link_len[v] = link_len[link[v]] + 1;
			ch[u][c] = v;
		}
		last = v;
		if (len[v] == ed - st + 1) first = v;
	}
	inline void addFirst(int st, int ed, int c) {
		auto get_node = [&](int x) {
			while (st+len[x]+1>ed || s[st+len[x]+1] != s[st])
				x = link[x];
			return x;
		};
		int u = get_node(first), v = ch[u][c];
		if (!v) {
			v = new_node(len[u] + 2);
			link[v] = ch[get_node(link[u])][c];
			link_len[v] = link_len[link[v]] + 1;
			ch[u][c] = v;
		}
		first = v;
		if (len[v] == ed - st + 1) last = v;
	}
	inline void init() {
		first = last = tot = 0;
		int rt = new_node(0);
		link[rt] = new_node(-1);
	}
};