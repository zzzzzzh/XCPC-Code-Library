namespace simple_path {
	//The number of connected blocks on the contour line will not exceed half of the number of columns plus 1
	using HashT = std::unordered_map<LL, int>;
	const int max_N = 12;
	struct state {
		int end, a[max_N], n, m;
		// add count of end point into state
		state(int col, LL s) : n(col + 1) {
			end = s & 3, s >>= 2; m = 0;
			for (int i = n - 1; ~i; --i)
				a[i] = (s & 7), s >>= 3, (a[i] > m) && (m = a[i]);
			++m;
		}
		void minimal() {
			static int f[max_N];
			memset(f, -1, sizeof(f));
			f[0] = 0, m = 1;
			for (int i = 0; i < n; ++i) {
				if (f[a[i]] == -1) f[a[i]] = m++;
				a[i] = f[a[i]];
			}
		}
		void add(int u, int v) { a[u] = a[v] = m++; }
		void addSingle(int u) { ++end, a[u] = m++; }
		void endSingle(int u) { ++end, a[u] = 0; }
		void unite(int u, int v) {
			int b = a[u], c = a[v];
			for (int i = 0; i < n; ++i) if (a[i] == b) a[i] = c;
			a[u] = a[v] = 0;
		}
		void swap(int u, int v) { std::swap(a[u], a[v]); }
		LL hash() {
			minimal(); LL s = 0;
			for (int i = 0; i < n; ++i) (s <<= 3) |= a[i];
			(s <<= 2) |= end;
			return s;
		}
		void move() {
			for (int i = n - 1; i; --i) a[i] = a[i - 1];
			a[0] = 0;
		}
	};
	HashT pre, cur;
	inline void updt(HashT &map, LL set, int tmp) {
		if (!map.count(set) || map[set] < tmp) map[set] = tmp;
	}
	int n, m, v[max_N][max_N];
	void solve() {
		int ans = INT_MIN;
		//input
		scanf("%d%d", &n, &m);
		for(int r = 0; r < n; ++r) for(int c = 0; c < m; ++c){
			scanf("%d", v[r] + c);
			ans = std::max(ans, v[r][c]);
		}
		pre.clear(), pre[0] = 0;
		for (int r = 0; r < n; ++r) {
			for (int c = 0; c < m; ++c) {
				//transfer grid (r, c)
				cur.clear();
				for (auto pr : pre) {
					state sta(m, pr.first);
					int a = sta.a[c], w = sta.a[c + 1];
					if (a && w) {
						//unite
						if (a != w) { // never unite two grid from the same connected block
							sta.unite(c, c + 1);
							updt(cur, sta.hash(), pr.second + v[r][c]);
						}
					} else if (!a && !w) {
						updt(cur, pr.first, pr.second);
						//add
						if (sta.end < 2) {
							state tmp = sta, ttmp = sta;
							tmp.addSingle(c);
							updt(cur, tmp.hash(), pr.second + v[r][c]);
							ttmp.addSingle(c + 1);
							updt(cur, ttmp.hash(), pr.second + v[r][c]);
						}
						sta.add(c, c + 1);
						updt(cur, sta.hash(), pr.second + v[r][c]);
					} else {
						if (sta.end < 2) {
							state tmp = sta;
							if (a) tmp.endSingle(c);
							else tmp.endSingle(c + 1);
							updt(cur, tmp.hash(), pr.second + v[r][c]);
						}
						//same
						updt(cur, pr.first, pr.second + v[r][c]);
						//swap
						sta.swap(c, c + 1);
						updt(cur, sta.hash(), pr.second + v[r][c]);
					}
				}
				pre = cur;
			}
			//transfer to next row
			cur.clear();
			for (auto pr : pre) {
				state sta(m, pr.first);
				if (!sta.a[m]) sta.move(), cur[sta.hash()] = pr.second;
			}
			pre = cur;
		}
		if (cur.count(2)) ans = std::max(ans, cur[2]);
		printf("%d\n", ans);
	}
}
