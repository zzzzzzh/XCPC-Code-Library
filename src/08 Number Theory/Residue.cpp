<TeX>二次、三次剩余模板。不过总感觉三次剩余有点问题，谁知道对不对呢...</TeX>
struct field2{
	int x, y, a, p;
	field2():x(0), y(0), a(0), p(0){}
	field2(int x,int y,int a,int p):x(x),y(y),a(a),p(p){}
	field2 operator * (const field2 &f)const{
		int retx=(1ll * x * f.x + 1ll * y * f.y % p * a) % p;
		int rety=(1ll * x * f.y + 1ll * y * f.x) % p;
		return field2(retx, rety, a, p);
	}
};
struct field3{
	int x, y, z, a, p;
	field3():x(0), y(0), z(0), a(0), p(0){}
	field3(int x,int y,z,a,p):x(x),y(y),z(z),a(a),p(p){}
	field3 operator * (const field3 &f)const{
		int retx = 1ll * x * f.x % p;
		retx = (retx + 1ll * y * f.z % p * a) % p;
		retx = (retx + 1ll * z * f.y % p * a) % p;
		int rety = (1ll * x * f.y + 1ll * y * f.x) % p;
		rety = (rety + 1ll * z * f.z % p * a) % p;
		int retz = (1ll * x * f.z + 1ll * z * f.x) % p;
		retz = (retz + 1ll * y * f.y) % p;
		return field3(retx, rety, retz, a, p);
	}
};
std::vector <int> remain2(int a, int p){
	if (!a || p == 2) return {a};
	if (powermod(a, p - 1 >> 1, p) != 1) return {};
    if (p == 3) return {1, 2};
	while (true){
		field2 f(randint(p-1) + 1, randint(p - 1) + 1, a, p);
		f = f.powermod(p - 1 >> 1);
		if (f.x) continue;
		int ret = powermod(f.y, p - 2, p);
		return {ret, p - ret};
	}
}
std::vector <int> remain3(int a, int p){
	if (!a || p <= 3) return {a};
	if (p%3==2) return {powermod(a, (2*p-1)/3, p)};
	while (true){
		field3 f(rdi(p-1)+1,rdi(p-1)+1,rdi(p-1)+1,a,p);
		f = f.powermod((p - 1) / 3);
		if (f.x || f.z) continue;
		int ret = powermod(f.y, p - 2, p);
		std::vector <int> vec = remain2(p - 3, p);
		int r=1ll*(vec[0]-1+p)%p*powermod(2,p-2,p)%p;
		std::vector <int> ans;
		for (int i = 0; i < 3; ++ i){
			ans.push_back(ret);
			ret = 1ll * ret * r % p;
		}
		return ans;
	}
}
