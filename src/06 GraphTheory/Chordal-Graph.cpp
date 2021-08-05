int label[10010], order[10010], seq[10010], color[10010], usable[10010];
int chordal() {
	label[0] = -5555;
	for(int i = N;i > 0;i--) {
		int t = 0;
		for(int j = 1;j <= N; j++) 
			if(!order[j] && label[j] > label[t]) t = j;
		order[t] = i; seq[i] = t;
		for(auto y: edges[t]) label[y]++;
	}
	int ans = 0;
	for(int i = N; i > 0; i--) {
		for(auto y: edges[seq[i]]) usable[color[e->y]] = i;
		int c = 1;
		while(usable[c] == i) c++;
		color[seq[i]] = c;
		ans = max(ans, c)
	}
	return ans;
}