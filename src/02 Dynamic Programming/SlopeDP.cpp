<TeX>优化形如 $dp[i]=\max(k_j*x[i]+b_j)$ 的dp，不要求b[j]单调。决策是若干条直线，动态维护一个下凸壳，复杂度$O(n\log{n})$</TeX>
const ll INF = LLONG_MAX;
inline ll divide(ll a, ll b){return a / b - ((a ^ b) < 0 && a % b);}
class L{
public:
	static bool isquery;
	mutable ll k, b, p;
	L (ll k = 0, ll b = 0, ll p = 0):k(k), b(b), p(p){}
	bool operator<(const L &l)const{return isquery?p<l.p:k<l.k;}
	ll gety(ll x)const{return k * x + b;}
};
bool L::isquery = false;
typedef std::multiset <L>::iterator iter;
bool over(std::multiset <L> &set, iter l1, iter l2){
	if (l2 == set.end()) return l1->p = INF, false;
	if (l1->k == l2->k) l1->p = l1->b > l2->b ? INF : -INF;
	else l1->p = divide(l2->b - l1->b, l1->k - l2->k);
	return l1->p >= l2->p;
}
void insert(std::multiset <L> &set, ll k, ll b){
	auto u = set.insert(L (k, b)), v = u ++, w = v;
	while (over(set, v, u)) u = set.erase(u);
	if (v != set.begin() && over(set, --w, v)) over(set, w, set.erase(v));
	while ((v = w) != set.begin() && (--w)->p >= v->p) over(set, w, set.erase(v));
}
ll query(std::multiset <L> &set, ll x){
	L::isquery = true;
	auto u = *(set.lower_bound(L (0, 0, x)));
	L::isquery = false;
	return u.gety(x);
}
