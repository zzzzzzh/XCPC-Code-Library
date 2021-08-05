<TeX>LCT maintains subtree size.Time: $O(n\log{n})$</TeX>
namespace LCT {
	using ptr = struct node *;
	ptr null, tail, rt[max_N];
	struct node {
		int val, rev; // int r_siz, v_siz;
		ptr max, p, ch[2];
		void updt() {
			max = this;
			if (ch[0]->max->val > max->val) max = ch[0]->max;
			if (ch[1]->max->val > max->val) max = ch[1]->max;
			// r_siz = v_siz + ch[0]->r_siz + ch[1]->r_siz;
		}
		void down() {
			if (!rev) return;
			std::swap(ch[0], ch[1]);
			ch[0]->rev ^= 1; ch[1]->rev ^= 1;
			rev = 0;
		}
		int dir() {
			if (p->ch[0] == this) return 0;
			if (p->ch[1] == this) return 1;
			return -1;
		}
		void set_ch(ptr x, int d) {
			if (~d) ch[d] = x;
			if (x != null) x->p = this;
		}
	} pool[max_N];
	ptr new_node(int val) {
		ptr x = tail++; // x->r_siz = x->v_siz = 1;
		x->val = val; x->rev = 0;
		x->ch[0] = x->ch[1] = x->p = null;
		return x->max = x;
	}
	void rot(ptr x) {
		ptr y = x->p;
		int d = x->dir(), dd = y->dir();
		y->p->set_ch(x, dd);
		y->set_ch(x->ch[d ^ 1], d); y->updt();
		x->set_ch(y, d ^ 1);
	}
	void putdown(ptr x) {
		if (~x->dir()) putdown(x->p);
		x->down();
	}
	void splay(ptr x) {
		putdown(x);
		int d, dd;
		for(; ~(d = x->dir()); rot(x))
			if (~(dd = x->p->dir())) rot((d ^ dd) ? x : x->p);
		x->updt();
	}
	void access(ptr x) {
		ptr y = x, rch = null;
		for (; x != null; x = x->p) {
			splay(x);
			// x->v_siz += x->ch[1]->r_siz;
			// x->v_siz -= rch->r_siz;
			x->set_ch(rch, 1); x->updt(); rch = x;
		}
		splay(y);
	}
	void evert(ptr x) {access(x), x->rev ^= 1;}
	ptr path(ptr x, ptr y) {evert(x), access(y); return y;}
	void link(ptr x, ptr y) {
		evert(x), x->p = y;
		// evert(x), access(y), x->p = y;
		// y->v_siz += x->r_siz, y->updt();
	}
	void cut(ptr x, ptr y) {
		path(x, y); x->p = null;
		y->set_ch(null, 0); y->updt();
	}
	void init() {
		tail = pool; null = tail++;
		null->val = null->rev = 0;
		// null->r_siz = null->v_siz = 0;
		null->ch[0] = null->ch[1] = null->p = null->max =null;
	}
}
