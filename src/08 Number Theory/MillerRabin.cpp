bool solve(int n, int t, int u){
	int a = random(n);
	if(!a || a == 1) return true;
	int pre = powermod(a, u, n);
	for (int i = 0; i < t; ++ i){
		int now = 1ll * pre * pre % n;
		if (now == 1 && pre != 1 && pre != n - 1) return false;
		pre = now;
	}
	return pre == 1;
}
bool solve(int n){
	if (n <= 30) return n == 2 || n == 3 || n == 5 || n == 7 || n == 11 || n == 13 || n == 17 || n == 19 || n == 23 || n == 29;
	if (!(n & 1)) return false;
	const int S = 7; int t = 0, x = n - 1;
	for ( ; !(x & 1); x >>= 1, ++ t);
	for (int i = 0; i < S; ++ i)
		if (!solve(n, t, x)) return false;
	return true;
}
