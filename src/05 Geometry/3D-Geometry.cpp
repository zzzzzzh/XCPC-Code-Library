<TeX>Based on \href{https://github.com/Sd-Invol/shoka/tree/master/CodeLibrary}{link}</TeX>
struct P{
	double x, y, z;
	explicit P (double x = 0, double y = 0, double z = 0):x(x), y(y), z(z){}
	P operator ^ (const P &p)const{return P (y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x);}
};
struct F{
	P p, o;
	F (){}
	F (P p, P o):p(p), o(o){}
	F (P p1, P p2, P p3):p(p1), o((p2 - p1) ^ (p3 - p1)){}
};
P projPF(P p, F f){
	double t = (f.o % (f.p - p)) / (f.o % f.o);
	return p + f.o * t;
}
double isLF(L l, F f){
	P u = f.p - l.p;
	return (u % f.o) / (l.v % f.o);
}
double disFP(F f, P p){ // 有向距离，q 在法向为正 
	return f.o % (p - f.p) / f.o.abs();
}
double area(P p1, P p2, P p3){
	return ((p2 - p1) ^ (p3 - p1)).abs();
}
bool inTri(P p1, P p2, P p3, P q){
	double area1 = area(q, p1, p2);
	double area2 = area(q, p2, p3);
	double area3 = area(q, p3, p1);
	return !sig(area1 + area2 + area3 - area(p1, p2, p3));
}
double mix(P p1, P p2, P p3) {return p1 % (p2 ^ p3);}
double isLL(L l1, L l2){ //共面时使用
	P u = l2.p - l1.p;
	P cross = u ^ l2.v, delta = l1.v ^ l2.v;
	if (sig(delta.z)) return cross.z / delta.z;
	if (sig(delta.y)) return cross.y / delta.y;
	if (sig(delta.x)) return cross.x / delta.x;
	return INF;
}
L isFF(F f1, F f2){
	P e = f1.o ^ f2.o, v = f1.o ^ e;
	double d = f2.o % v;
	if (!sig(d)) return L(P(INF,INF,INF),P(INF,INF,INF));
	P q = f1.p + v * ((f2.o % (f2.p - f1.p)) / d);
	return L (q, q + e);
}
P rotate(P a, P b, double angle){
	P e1, e2, e3;
	b = b / b.abs(), e3 = b;
	double lens = a % e3;
	e1 = a - e3 * lens;
	if (sig(e1.abs()) > 0) e1 = e1 / e1.abs();
	else return a;
	e2 = e1 ^ e3;
	double x1 = a % e2, y1 = a % e1, x2, y2;
	x2 = x1 * cos(angle) - y1 * sin(angle);
	y2 = x1 * sin(angle) + y1 * cos(angle);
	return e3 * lens + e1 * y2 + e2 * x2;
}
/*绕任意轴（过原点）逆时针旋转（注意要把轴向量归一化，不然点在轴上时会出问题） 
	rotate x y z d, let t=(1-cos(d))
   | txx+cos(d)     txy+sin(d)*z   txz-sin(d)*y   0 |
   | tyx-sin(d)*z   tyy+cos(d)     tyz+sin(d)*x   0 |
   | tzx+sin(d)*y   tzy-sin(d)*x   tzz+cos(d)     0 |
   |    0              0              0           1 | */
double onLeft(L l, P p, P o){ // >0表示 p 在 l 左边
	return mix(o, p - l.p, l.v);
}
std::vector <P> convexHull2D(std::vector <P> &ps, P o){
	const int N = 100010;
	static P stack[N];
	std::sort(ps.begin(), ps.end());
	int n = ps.size(), top = 0;
	for (int i = 0; i < n; ++ i){
		while (top > 1 && sig(onLeft(L (stack[top - 2], stack[top - 1]), ps[i])) <= 0)
			--top;
		stack[top ++] = ps[i];
	}
	int tmp = top;
	for (int i = n - 2; i >= 0; -- i){
		while (top > tmp && sig(onLeft(L (stack[top - 2], stack[top - 1]), ps[i])) <= 0)
			-- top;
		stack[top ++] = ps[i];
	}
	if (n > 1) -- top;
	std::vector <P> ret;
	for (int i = 0; i < top; ++i) ret.push_back(stack[i]);
	return ret;
}
std::vector<vector<P>> convexHull(vector <P> &ps){
	auto area = [ps](int a, int b, int c){
		return ((ps[b] - ps[a]) ^ (ps[c] - ps[a])).abs();
	};
	auto volume = [&](int a, int b, int c, int d){
		return mix(ps[b]-ps[a], ps[c]-ps[a], ps[d]-ps[a]);
	};
	struct Face{
		int a[3];
		Face(){}
		Face(int x, int y, int z){a[0]=x, a[1]=y, a[2]=z;}
		int &operator [](int k){return a[k];}
	};
	std::vector <Face> face;
	std::sort(ps.begin(), ps.end());
	ps.erase(std::unique(ps.begin(), ps.end()), ps.end());
	std::random_shuffle(ps.begin(), ps.end());
	int n = ps.size();
	auto find = [&](){
		int ret = -1;
		for (int i = 2; i < n; ++ i){
			P dir = F (ps[i], ps[0], ps[1]).o;
			if (dir == P ()) continue;
			std::swap(ps[i], ps[2]);
			ret = 0;
			for (int j = i + 1; j < n; ++ j)
				if (sig(volume(0, 1, 2, j))) {
					std::swap(ps[j], ps[3]);
					face.emplace_back(0, 1, 2);
					face.emplace_back(0, 2, 1);
					return 1;
				}
		}
		return ret;
	};
	std::vector <std::vector <P>> ret;
	int state = find();
	if (state == 1) {
		std::vector <std::vector <int>> mark(n + 5);
		for (auto &u : mark) u.resize(n + 5);
		int cnt = 0;
		for (int i = 3; i < n; ++ i){
			std::vector <Face> tmp;
			++ cnt;
			for (auto u : face){
				int a = u[0], b = u[1], c = u[2];
				if (sig(volume(i, a, b, c)) < 0){
					mark[a][b] = mark[b][a] = mark[b][c] = 
					mark[c][b] = mark[c][a] = mark[a][c] = cnt;
				}
				else tmp.push_back(u);
			}
			face = tmp;
			for (int j = 0, sz = tmp.size(); j < sz; ++ j){
				int a=face[j][0], b=face[j][1], c=face[j][2];
				if (mark[a][b] == cnt) face.emplace_back(b, a, i);
				if (mark[b][c] == cnt) face.emplace_back(c, b, i);
				if (mark[c][a] == cnt) face.emplace_back(a, c, i);
			}
		}
		typedef std::pair <P, Face> Faces;
		std::vector <Faces> faces;
		for (auto u : face){
			P p = F(ps[u[1]], ps[u[0]], ps[u[2]]).o;
			p = p / p.abs();
			faces.push_back({p, u});
		}
		std::sort(faces.begin(), faces.end(), [](const Faces &f1, const Faces &f2){return f1.first < f2.first;});
		std::vector <P> pss;
		for (int i = 0, sz = faces.size(); i <= sz; ++ i) {
			if (i == sz || i > 0 && !(faces[i].first == faces[i - 1].first)){
				ret.push_back(convexHull2D(pss,faces[i-1].first));
				pss.clear();
			}
			if (i < sz){
				pss.push_back(ps[faces[i].second[0]]);
				pss.push_back(ps[faces[i].second[1]]);
				pss.push_back(ps[faces[i].second[2]]);
			}
		}
	} else if (!state) ret.push_back(convexHull2D(ps, F (ps[0], ps[1], ps[2]).o));
	else if (ps.size() < 2) ret.push_back(ps);
	else {
		P dir = ps[1] - ps[0], o;
		if (sig(dir.x)) o = P(-dir.y, dir.x, 0);
		else o = P (0, -dir.z, dir.y);
		ret.push_back(convexHull2D(ps, o));
	}
	return ret;
}
vector<vector<P>> cutCon(vector<vector <P>> &ps, F f){
	std::vector <std::vector <P>> ret;
	std::vector <P> sec;
	for (auto &u : ps){
		int n = u.size();
		std::vector <P> qs;
		bool dif = false;
		for (int i = 0; i < n; ++ i){
			P p1 = u[i], p2 = u[(i + 1) % n];
			int d1 = sig(f.o % (p1 - f.p));
			int d2 = sig(f.o % (p2 - f.p));
			if (d1 <= 0) qs.push_back(p1);
			if (d1 * d2 < 0){
				L l(p1, p2);
				double t = isLF(l, f);
				P q = l.point(t);
				qs.push_back(q); sec.push_back(q);
			}
			if (!d1) sec.push_back(p1);
			else dif = true;
			dif |= sig(mix(f.o, p2-p1, u[(i+2)%n] - p1)) < 0;
		}
		if (qs.size() > 0 && dif) ret.push_back(qs);
	}
	if(sec.size()>0) ret.push_back(convexHull2D(sec, f.o));
	return ret;
}
