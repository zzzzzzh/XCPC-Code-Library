struct frac{
	int x, y;
	frac(int x, int y){
		int gcd = std::__gcd(x, y);
		x /= gcd; y /= gcd;
		if (y < 0){x = -x;y = -y;}
		this->x = x; this->y = y;
	}
	frac operator + (const frac &f) const {return frac(x * f.y + y * f.x, y * f.y);}
	frac operator - (const frac &f) const {return frac(x * f.y - y * f.x, y * f.y);}
	frac operator * (const frac &f) const {return frac(x * f.x, y * f.y);}
	frac operator / (const frac &f) const {return frac(x * f.y, y * f.x);}
	bool operator < (const frac &f) const {return x * f.y < y * f.x;}
};
