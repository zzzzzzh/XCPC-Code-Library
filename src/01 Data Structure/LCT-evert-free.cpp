<TeX>LCT without evert operation. Time: $O(n\log{n})$</TeX>
namespace lct {
	using star = struct node *;
#define Siz(x) ((x) ? (x)->siz : 0)
	struct node {
		int siz; star p, ch[2];
		void update() {siz = Siz(ch[0]) + 1 + Siz(ch[1]);}
		int dir() {
			if (!p)return -1;
			if (p->ch[0] == this)return 0;
			if (p->ch[1] == this)return 1;
			return -1;
		}
		void set(star x, int d) {
			if (~d && this)ch[d] = x;
			if (x)x->p = this;
		}
	} pool[max_N], *tail = pool;
	star new_node() {
		star x = tail++; x->siz = 1;
		x->ch[0] = x->ch[1] = x->p = 0;
		return x;
	}
	void rot(star x) {
		star y = x->p;
		int d = x->dir(), d1 = y->dir();
		y->p->set(x, d1);
		y->set(x->ch[d ^ 1], d), y->update();
		x->set(y, d ^ 1);
	}
	void splay(star x) {
		for (int d, d1; ~(d = x->dir()); rot(x))
			if (~(d1 = x->p->dir())) rot((d ^ d1) ? x : x->p);
		x->update();
	}
	void access(star x) {
		star rch = 0;
		for (star y = x; y; rch = y, y = y->p)
			splay(y), y->set(rch, 1), y->update();
		splay(x);
	}
	void link(star x, star y) { // p[x]=y
		access(x), assert(!x->p), x->p = y;
	}
	void cut(star x, star y) { // p[x]=y
		access(x); x->ch[0]->p = 0;
		x->ch[0] = 0; x->update();
	}
	int kth(star x, int k) {
		while (x) {
			int rk = Siz(x->ch[0]) + 1;
			if (k == rk) break;
			if (k < rk)x = x->ch[0];
			else k -= rk, x = x->ch[1];
		}
		splay(x);
		return x - pool;
	}
	int find(star x) {return access(x), kth(x, 1);}
}
