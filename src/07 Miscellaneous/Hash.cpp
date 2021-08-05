hash<int> hasher;
 
ll hsh (ll a, int b){
	return a ^ (hasher(b) + 0x9e3779b9 + (a << 6) + (a >> 2));
}

int main() {
    ll f = 0x293859823253;
	for (int i = 0; i < n; ++) {
        f = hsh(f, a[i]);
	}
    return 0;
}
