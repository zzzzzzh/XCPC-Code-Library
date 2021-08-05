int solve(int n){//n是1时会除0
	int c = random(n - 1) + 1, x = random(n), y = x, i = 0, k = 1;
	for ( ; ; ){
		++ i;
		x = (1ll * x * x + c) % n;
		int d = std::__gcd(std::abs(y - x), n);
		if (d != 1 && d != n) return d;
		if (x == y) return n;//小心这里会产生1
		if (i == k){ y = x; k <<= 1; }
	}
}

void Solve(int n){
    if (n <= 1) return;
    if (millerrabin(n)){
        fact.push_back(n);
        return;
    }
    int x = solve(n);
    Solve(x);
    Solve(n / x);
}

