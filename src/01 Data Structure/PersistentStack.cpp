namespace persistent_stack { // min x + ky
    const int max_N = (int) 1e5 + 21;
    const int log_N = 20;

    int x[max_N], y[max_N], dep[max_N], bit[max_N], st[max_N][log_N];
	// bit[1] = 0, bit[i] = bit[i >> 1] + 1;

    inline int calc(int u, int k) {
        for (int i = log_N - 1; i > 1; --i) {
            int r = st[u][i - 1];
            if (!r) continue;
            int l = st[u][i];
            if (!l) {
				l = st[r][bit[dep[r]]];
				if (!l) continue;
			}
            int fl = x[l] + k * y[l];
            int fr = x[r] + k * y[r];
            if (fl < fr) {
                u = r;
                ++i;
            }
        }
        int ret = INT_MAX;
        for (int i = 0; i < 4 && u; ++i, u = st[u][0]) {
            ret = std::min(ret, x[u] + k * y[u]);
        }
        return ret;
    }

    inline bool onLeft(int p1, int p2, int q) {
        int x1 = x[p2] - x[p1], y1 = y[p2] - y[p1];
        int x2 = x[q] - x[p1], y2 = y[q] - y[p1];
        return (x1 * y2 - x2 * y1) < 0;
    }

    inline int get_pre(int u, int v) {
        for (int i = log_N - 1; ~i; --i) {
            int p = st[u][i], pp = st[p][0];
            if (!p || !pp) continue;
            if (onLeft(pp, p, v)) u = p;
        }
        if (st[u][0] && onLeft(st[u][0], u, v)) {
            u = st[u][0];
        }
        return u;
    }
};