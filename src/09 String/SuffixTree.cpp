int len, tot, rt, curP, curC, curL, rem; 
struct Node {
	int L, R, fail, nxt[maxc + 1];
} e[maxn << 1];
char buf[maxn];
int newNode(int L, int R) { // [L, R)
	e[tot] = (Node){L, R, rt}; // nxt = {} return tot ++;
	return tot++;
}
void append(char ch) { 
	buf[len++] = ch, ++rem; 
	for(int las = 0; rem; --rem) {
		!curL && (curC = len - 1);
		int p = e[curP].nxt[buf[curC] - 'a']; 
		while(p && curL >= e[p].R - e[p].L) {
			curC += e[p].R - e[p].L;
			curL -= e[p].R - e[p].L;
			curP = p;
			p = e[curP].nxt[buf[curC] - 'a'];
		}
		if(!p) {
			e[curP].nxt[buf[curC]-'a']=newNode(len-1,maxn);
			last && (e[last].fail = curP);
			last = curP;
		} else if(buf[e[p].L + curL] == ch) {
			++curL;
			last && (e[last].fail = curP);
			break;
		} else {
			int u = newNode(e[p].L, e[p].L + curL);
			int v = newNode(len - 1, maxn);
			e[curP].nxt[buf[curC] - 'a'] = u;
			e[p].L += curL; 
			e[u].nxt[buf[e[p].L] - 'a'] = p; 
			e[u].nxt[ch - 'a'] = v;
			last && (e[last].fail = u);
			last = u;
		}
		curP==rt&&curL?(--curL,++curC):(curP=e[curP].fail);
	}
}
// curP = rt = Node(0, 0), e[rt].fail = rt, curL = 0, append(’$’)