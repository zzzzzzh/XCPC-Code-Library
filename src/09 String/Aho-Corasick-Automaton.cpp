struct AC_DFA{
	int go[MAXNODE][DICSIZE], fail[MAXNODE], sz, root;
	int newnode(){
		int ret = ++sz;
		memset(go[ret], 0, sizeof(go[ret]));
		fail[ret] = root; //f[ret] = 0;
		return ret;
	}
	void init(){
		root = sz = 0; root = newnode();
	}
	void insert(char *str, int v){
		int n = strlen(str), p = root;
		for(int i = 0; i < n; ++ i){
			int c = str[i] - 'a';
			if(!go[p][c]) go[p][c] = newnode();
			p = go[p][c];
		} //f[p] |= 1 << v;
	}
	void makefail(){
		std::queue<int> queue;
		queue.push(root);
		while(!queue.empty()){
			int p = queue.front(); queue.pop();
			for(int i = 0; i < DICSIZE; ++ i)
				if(go[p][i]){
					fail[go[p][i]] = fail[p] ? go[fail[p]][i]:root;
					int q = go[p][i]; //f[q] |= f[fail[q]];
					queue.push(q);
				}
				else go[p][i] = p == root ? root : go[fail[p]][i];
		}}};
