<TeX>dlx 可重覆盖模板，行和列的下标从 $0$ 开始，一开始调用 init(R,C) 初始化，$R$ 为行数，$C$ 为列数。在 $x$ 行 $y$ 列添加节点直接调用 add(x,y)。
如果行带权的话存 $val$ 数组，下标同样从 $0$ 开始，慢的话看情况加最优化剪枝</TeX>
namespace dlx_with_overlapping{
	const int N = 55, R = N, C = 55, MaxNode = R * C;
	struct node {
		int x, y;
		node *l, *r, *u, *d;
	};
	node nodes[MaxNode], *nxt, *root, *row[R], *col[C];
	int size[C];
	bool mark[C];
	void init(int r, int c) {
		nxt = nodes;
		memset(row, 0, sizeof(row));
		memset(size, 0, sizeof(size));
		root = nxt++;
		root->l = root->r = root;
		for (int y = 0; y < c; ++y) {
			node *p = nxt++;
			p->x = -1; p->y = y;
			p->r = root; p->l = root->l;
			p->r->l = p->l->r = p;
			col[y] = p->u = p->d = p;
		}
	}
	node *add(int x, int y) {
		node *p = nxt++;
		p->x = x; p->y = y;
		size[y]++;
		if (!row[x]) row[x] = p->l = p->r = p;
		else {
			p->r = row[x]; p->l = row[x]->l;
			p->r->l = p->l->r = p;
		}
		p->d = col[y]; p->u = col[y]->u;
		p->u->d = p->d->u = p;
		return p;
	}
	void cover(node *x) {
		for (node *y = x->d; y != x; y = y->d) {
			y->l->r = y->r; y->r->l = y->l;
			size[x->y]--;
		}
	}
	void uncover(node *x) {
		for (node *y = x->u; y != x; y = y->u) {
			y->l->r = y->r->l = y;
			size[x->y]++;
		}
	}
	int h() {
		int res = 0;
		node *x, *y, *z;
		memset(mark, 0, sizeof(mark));
		for (x = root->l; x != root; x = x->l) if (!mark[x->y]) {
			mark[x->y] = 1; res++;
			for (y = x->u; y != x; y = y->u)
				for (z = y->r; z != y; z = z->r)
					mark[z->y] = 1;
		}
		return res;
	} //贪心确定上界
	int ans, val[R];
	void dfs(int dep, int curval)  {
		if (curval >= ans) return ;
		node *x, *y, *z = NULL;
		if (dep < h()) return ;
		for (x = root->r; x != root; x = x->r)
			if (!z || size[x->y] < size[z->y]) z = x;
		if (!z) {
			ans = min(ans, curval); return;
		}
		if (!dep) return ;
		for (x = z->u; x != z; x = x->u) {
			cover(x);
			for (y = x->r; y != x; y = y->r) cover(y);
			dfs(dep - 1, curval + val[x->x]);
			for (y = x->l; y != x; y = y->l) uncover(y);
			uncover(x);
		}
	}
	int solve(int MAX, int *_val)  {
		ans = INF;
		memcpy(val, _val, sizeof(val));
		dfs(MAX, 0);
		return ans;
	}
}