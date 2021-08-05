namespace circuits {
	//The number of connected blocks on the contour line will not exceed half of the number of columns
	using HashT = std::unordered_map<LL, int>;
	const int max_N = 12;
	struct state {
		int a[max_N], n, m;
		state(int col, LL s) : n(col + 1) {
			m = 0;
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
		void unite(int u, int v) {
			int b = a[u], c = a[v];
			for (int i = 0; i < n; ++i) if (a[i] == b) a[i] = c;
			a[u] = a[v] = 0;
		}
		void swap(int u, int v) { std::swap(a[u], a[v]); }
		LL hash() {
			minimal(); LL s = 0;
			for (int i = 0; i < n; ++i) (s <<= 3) |= a[i];
			return s;
		}
		void move() {
			for (int i = n - 1; i; --i) a[i] = a[i - 1];
			a[0] = 0;
		}
	};
	HashT pre, cur;
	void updt(HashT &map, LL set, int tmp) {
		if (!map.count(set) || map[set] > tmp) map[set] = tmp;
	}
	void solve(int n, int m) {
		//input
		pre.clear(), pre[0] = 0;
		for (int r = 0; r < n; ++r) {
			for (int c = 0; c < m; ++c) {
				//transfer grid (r, c)
				cur.clear();
				for (auto pr : pre) {
					state sta(m, pr.first);
					int a = sta.a[c], w = sta.a[c + 1];
					if (a && w) // unite
					else if (!a && !w) // add
					else // same, swap
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
		assert(cur.count(0));
		printf("%d\n", cur[0]);
	}
}
