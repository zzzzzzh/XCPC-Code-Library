<TeX>$x, y$ 满足 $|x|+|y|$ 最小</TeX>
int ex_euc(int a, int b, int &x, int &y){
	if (!b){ x = 1, y = 0; return a; }
	int ret = ex_euc(b, a % b, x, y), tmp = y;
	y = x - a / b * y; x = tmp;
	return ret;
}
int inv(int a, int moder){
	int b = moder, s = 1, t = 0;
	while (b){
		int tmp = a, q = a / b;
		a = b, b = tmp % a;
		tmp = s; s = t; t = tmp - s * q;
	}
	if (a > 1) return -1;
	return s < 0 ? s + moder : s;
}
