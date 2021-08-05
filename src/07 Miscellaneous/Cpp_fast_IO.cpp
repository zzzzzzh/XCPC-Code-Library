namespace fastIO {
#define BSIZE (100000)
	bool IOerror = 0;
	inline char nc() {
		static char buf[BSIZE], *p1 = buf + BSIZE, *pend = p1;
		if (p1 == pend) {
			p1 = buf; pend = buf + fread(buf, 1, BSIZE,stdin);
			if (pend == p1) {IOerror = 1; return -1;}
		}
		return *p1++;
	}
	inline void read(int &x) {
		char ch; int f = 1;
		while(isspace(ch = nc())||ch=='-') if(ch=='-') f=-1;
		if (IOerror) return;
		for (x=ch-'0'; isdigit(ch = nc()); x=x*10+ch-'0');
		x *= f;
	}
#undef BSIZE
}
