<TeX>Based on \href{https://github.com/Sd-Invol/shoka/tree/master/CodeLibrary}{link}</TeX>
#include<bits/stdc++.h>

const double eps = 1e-9;
const double PI = acos(-1.0);
const double INF = 1e100;

inline int sig(double p){
    return (p > eps) - (p < -eps);
}

inline double sqr(double p){return p * p;}

class P{
public:
    double x, y;
    explicit P (double x = 0, double y = 0): x(x), y(y){}
    P operator + (const P &p)const{return P (x + p.x, y + p.y);}
    P operator - (const P &p)const{return P (x - p.x, y - p.y);}
    P operator * (const double &p)const{return P (x * p, y * p);}
    friend P operator * (const double &d, const P &p){return P (d * p.x, d * p.y);}
    P operator / (const double &p)const{return P (x / p, y / p);}
    double operator ^ (const P &p)const{return x * p.y - y * p.x;}
    double operator % (const P &p)const{return x * p.x + y * p.y;}
    double abs2()const{return *this % *this;}
    double abs()const{return std::sqrt(abs2());}
    double angle()const{return std::atan2(y, x);}

    // 逆时针旋转 rad 弧度
    P rot(const double &rad)const{
        double sin = std::sin(rad), cos = std::cos(rad);
        return P (x * cos - y * sin, x * sin + y * cos);
    }

    P rot90()const{
        return P (-y, x);
    }

    bool operator < (const P &p)const{
        if (sig(x - p.x)) return x < p.x;
        return y < p.y;
    }

    bool operator == (const P &p)const{
        return !sig(x - p.x) && !sig(y - p.y);
    }

    void read(){
        scanf("%lf%lf", &x, &y);
    }

    void write(){
        printf("%.10lf %.10lf\n", x, y);
    }
};

class L{
public:
    P p, v;
    double angle;

    L (){}
    L (P a, P b):p(a), v(b - a){angle = std::atan2(v.y, v.x);}

    bool operator < (const L &l)const{
        return angle < l.angle;
    }

    P point(double t){
        return p + v * t;
    }
};

class C{
public:
    P o;
    double r;

    C (){}
    C (P o, double r):o(o), r(r){}

    P point(double angle){
        return P (o + P (std::cos(angle), std::sin(angle)) * r);
    }
};

double rad(P p1, P p2){
    return std::acos((p1 % p2) / (p1.abs() * p2.abs()));
}

P normal(P p){
    return p.rot90() / p.abs();
}

// v1 和 v2 是两直线的方向向量
double isLL(L l1, L l2){
    P u = l2.p - l1.p;
    return (u ^ l2.v) / (l1.v ^ l2.v); // 返回交点在 p1v1 上的位置，可以用来判断射线、线段等
}

// 返回有向距离，q 在直线逆时针方向为正
double disLP(L l, P p){
    return l.v ^ (p - l.p) / l.v.abs();
}

double disSP(P p1, P p2, P q){
    if (p1 == p2) return (q - p1).abs();
    P v1 = p2 - p1, v2 = q - p1, v3 = q - p2;
    if (sig(v1 % v2) < 0) return v2.abs();
    if (sig(v1 % v3) > 0) return v3.abs();
    return std::abs(v1 ^ v2) / v1.abs();
}

P proj(L l, P p){
    return l.p + l.v * (l.v % (p - l.p) / l.v.abs2());
}

P symmetry(L l, P p){
    return 2 * proj(l, p) - p;
}

//判断线段是否严格相交（包括严格重合）
bool crsSS(P p1, P p2, P q1, P q2){
    double c1 = (p2 - p1) ^ (q1 - p1);
    double c2 = (p2 - p1) ^ (q2 - p1);
    if (!sig(c1) && !sig(c2)){
        if (p2 < p1) std::swap(p1, p2);
        if (q2 < q1) std::swap(q1, q2);
        return std::max(p1, q1) < std::min(p2, q2);
    }
    double c3 = (q2 - q1) ^ (p1 - q1);
    double c4 = (q2 - q1) ^ (p2 - q1);
    return sig(c1) * sig(c2) < 0 && sig(c3) * sig(c4) < 0;
}

bool onSeg(P p1, P p2, P q){
    double len = (q - p1).abs();
    if (!sig(len)) return true;
    p1 = p1 - q, p2 = p2 - q;
    return !sig((p1 ^ p2) / len) && sig(p1 % p2) <= 0;
}

bool onRay(P p1, P p2, P q){
    double len = (q - p1).abs();
    if (!sig(len)) return true;
    p1 = q - p1, p2 = q - p2;
    return !sig((p1 ^ p2) / len) && sig(p1 % (p2 - p1)) >= 0;
}

std::vector <double> isCL(C c1, L l){ // 以离 l.p 近为序
    double a = l.v.x, b = l.p.x - c1.o.x, c = l.v.y, d = l.p.y - c1.o.y;
    double e = sqr(a) + sqr(c), f = 2 * (a * b + c * d);
    double g = sqr(b) + sqr(d) - sqr(c1.r);
    double delta = sqr(f) - 4 * e * g;
    if (sig(delta) < 0) return {};
    if (!sig(delta)){
        return {-f / (2 * e)};
    }
    std::vector <double> ret;
    ret.push_back((-f - std::sqrt(delta)) / (2 * e));
    ret.push_back((-f + std::sqrt(delta)) / (2 * e));
    return ret;
}

// 重合:-1，内含:0，内切:1，相交:2，外切:3，相离:4
std::pair <int, std::vector <P>> isCC(C c1, C c2){
    double d = (c1.o - c2.o).abs2();
    if (!sig(d)){
        if (!sig(c1.r - c2.r)){
            return {-1, {}};
        }
        return {0, {}};
    }
    if (sig(c1.r + c2.r - std::sqrt(d)) < 0) return {4, {}};
    if (sig(std::abs(c1.r - c2.r) - std::sqrt(d)) > 0) return {0, {}};
    double x = ((sqr(c1.r) - sqr(c2.r)) / d + 1) / 2;
    double y = std::max(sqr(c1.r) / d - sqr(x), 0.0);
    P q1 = c1.o + (c2.o - c1.o) * x;
    P q2 = ((c2.o - c1.o) * std::sqrt(y)).rot90();
    if (!sig(y)){
        return {!sig(c1.r + c2.r - std::sqrt(d)) ? 3 : 1, {q1}};
    }
    return {2, {q1 - q2, q1 + q2}};
}

int isCs(std::vector <C> &c, double mid){
    int n = c.size();
    double left = -INF, right = INF;
    for (int i = 0; i < n; ++ i){
        if (sig(mid - c[i].o.x - c[i].r) > 0) return 1;
        if (sig(c[i].o.x - c[i].r - mid) > 0) return 2;
        double delta = std::sqrt(std::max(0.0, sqr(c[i].r) - sqr(mid - c[i].o.x)));
        left = std::max(left, c[i].o.y - delta);
        right = std::min(right, c[i].o.y + delta);
        if (sig(left - right) > 0){
            for (int j = 0; j < i; ++ j){
                auto u = isCC(c[i], c[j]);
                auto ps = u.second;
                if (u.first <= 1) continue;
                if (u.first == 4) return 1;
                if (u.first == 3){
                    if (!sig(ps[0].x - mid)) continue;
                    return sig(ps[0].x - mid) < 0 ? 1 : 2;
                }
                if (!sig(ps[0].x - mid) || sig(ps[1].x - mid) != sig(ps[0].x - mid)) continue;
                return sig(ps[0].x - mid) < 0 ? 1 : 2;
            }
        }
    }
    return 0;
}

bool isCs(std::vector <C> &c){
    double right = -INF, left = INF;
    for (auto u : c){
        right = std::max(right, u.o.x + u.r);
        left = std::min(left, u.o.x - u.r);
    }
    for (int i = 0; i < 50; ++ i){
        double mid = (left + right) / 2;
        int x = isCs(c, mid);
        if (!x) return true;
        if (x == 1) right = mid;
        else left = mid;
    }
    return false;
}

double areaCC(C c1, C c2){
    double d = (c1.o - c2.o).abs();
    if (sig(c1.r + c2.r - d) <= 0){
        return 0;
    }
    if (sig(d - std::abs(c1.r - c2.r)) <= 0){
        return sqr(std::min(c1.r, c2.r)) * PI;
    }
    double x = (sqr(d) + sqr(c1.r) - sqr(c2.r)) / (2 * d);
    double t1 = std::acos(x / c1.r);
    double t2 = std::acos((d - x) / c2.r);
    return sqr(c1.r) * t1 + sqr(c2.r) * t2 - d * c1.r * sin(t1);
}

std::vector <P> tanCP(C c, P p){ // p在圆上时返回 p 点本身
    double d = (p - c.o).abs2();
    double x = d - sqr(c.r);
    if (sig(x) < 0) return {};
    if (!sig(x)) return {p};
    P q1 = (p - c.o) * sqr(c.r) / d;
    P q2 = ((p - c.o) * -c.r * std::sqrt(x) / d).rot90();
    return {c.o + q1 - q2, c.o + q1 + q2};
}

std::vector <L> tanCC(C c1, C c2){
    std::vector <L> ret;
    if (!sig(c1.r - c2.r)){
        P dir = c2.o - c1.o;
        dir = (dir * c1.r / dir.abs()).rot90();
        ret.push_back(L (c1.o + dir, c2.o + dir));
        ret.push_back(L (c1.o - dir, c2.o - dir));
    }
    else{
        P p = (c1.o * -c2.r + c2.o * c1.r) / (c1.r - c2.r);
        std::vector <P> ps = tanCP(c1, p);
        std::vector <P> qs = tanCP(c2, p);
        for (int i = 0; i < ps.size() && i < qs.size(); ++ i){
            ret.push_back(L (ps[i], qs[i]));
        }
    }
    P p = (c1.o * c2.r + c2.o * c1.r) / (c1.r + c2.r);
    std::vector <P> ps = tanCP(c1, p);
    std::vector <P> qs = tanCP(c2, p);
    for (int i = 0; i < ps.size() && i < qs.size(); ++ i){
        ret.push_back(L (ps[i], qs[i]));
    }
    for (auto &u : ret){
        if (u.v == P (0, 0)){
            u.v = (u.p - c1.o).rot90();
        }
    }
    return ret;
}

C inC(P p1, P p2, P p3){
    double a = (p3 - p2).abs(), b = (p1 - p3).abs(), c = (p2 - p1).abs();
    P p = (p1 * a + p2 * b + p3 * c) / (a + b + c);
    return C (p, std::abs(disLP(L (p1, p2), p)));
}

C outC(P p1, P p2, P p3){
    P q1 = (p1 + p2) / 2, v1 = normal(p1 - p2);
    P q2 = (p2 + p3) / 2, v2 = normal(p2 - p3);
    L l1(q1, q1 + v1), l2(q2, q2 + v2);
    double t = isLL(l1, l2);
    P p = l1.point(t);
    return C (p, (p1 - p).abs());
}

// 内部:1, 外部:-1, 边上:0
int inPolygon(std::vector <P> &ps, P q){
    int n = ps.size();
    for (int i = 0; i < n; ++ i){
        if (onSeg(ps[i], ps[(i + 1) % n], q)){
            return 0;
        }
    }
    int ret = -1;
    for (int i = 0; i < n; ++ i){
        P a = ps[i], b = ps[(i + 1) % n];
        if (a.y > b.y) std::swap(a, b);
        if (sig((a - q) ^ (b - q)) < 0 && sig(a.y - q.y) < 0 && sig(b.y - q.y) >= 0){
            ret = -ret;
        }
    }
    return ret;
}

// >0表示 q 在 l 左边
double onLeft(L l, P p){
    return l.v ^ (p - l.p);
}

std::vector <P> convexHull(std::vector <P> &ps){ //返回的凸包为逆时针方向
    const int N = 100010; // 抄时记得修改
    static P stack[N];
    std::sort(ps.begin(), ps.end());
    int n = ps.size(), top = 0;
    for (int i = 0; i < n; ++ i){
        while (top > 1 && sig(onLeft(L (stack[top - 2], stack[top - 1]), ps[i])) <= 0){
            -- top;
        }
        stack[top ++] = ps[i];
    }
    int tmp = top;
    for (int i = n - 2; i >= 0; -- i){
        while (top > tmp && sig(onLeft(L (stack[top - 2], stack[top - 1]), ps[i])) <= 0){
            -- top;
        }
        stack[top ++] = ps[i];
    }
    if (n > 1) -- top;
    std::vector <P> ret;
    for (int i = 0; i < top; ++ i){
        ret.push_back(stack[i]);
    }
    return ret;
}

std::vector <P> minkow_sum(const std::vector <P> &pps, const std::vector <P> &qqs){
    std::vector <P> ps = pps, qs = qqs;
    int n = ps.size(), m = qs.size();
    ps.emplace_back(ps[0]), qs.emplace_back(qs[0]);
    std::vector <P> ret;
    P cur = ps[0] + qs[0];
    for (int i = 0, j = 0; i < n || j < m; ){
        if (i < n){
            if (j == m){
                cur = cur + ps[i + 1] - ps[i];
                ++ i;
            }
            else{
                double det = (ps[i + 1] - ps[i]) ^ (qs[j + 1] - qs[j]);
                if (sig(det) > 0){
                    cur = cur + ps[i + 1] - ps[i];
                    ++ i;
                }
                else if (sig(det) == 0){
                    cur = cur + ps[i + 1] - ps[i] + qs[j + 1] - qs[j];
                    ++ i, ++ j;
                }
                else{
                    cur = cur + qs[j + 1] - qs[j];
                    ++ j;
                }
            }
        }
        else{
            cur = cur + qs[j + 1] - qs[j];
            ++ j;
        }
        ret.emplace_back(cur);
    }
    std::rotate(ret.begin(), ret.end() - 1, ret.end());
    return ret;
}

bool inConvex(std::vector <P> &ps, P q){ //O(logn)
    int n = ps.size();
    if (sig(onLeft(L (ps[0], ps[1]), q)) < 0 || sig(onLeft(L (ps[0], ps[n - 1]), q) > 0)){
        return 0;
    }
    int left = 2, right = n - 1;
    while (left < right){
        int mid = left + right >> 1;
        if (sig(onLeft(L (ps[0], ps[mid]), q)) <= 0){
            right = mid;
        }
        else{
            left = mid + 1;
        }
    }
    return sig(onLeft(L (ps[right - 1], ps[right]), q)) >= 0;
}

int tanConPR(std::vector <P> &ps, P q){ //O(logn)
    int ret = 0, n = ps.size();
    int left = 1, right = n - 1;
    while (left <= right){
        bool dnl = onLeft(L (q, ps[left + 1]), ps[left]) > 0;
        int mid = left + right >> 1;
        bool dnm = onLeft(L (q, ps[mid + 1]), ps[mid]) > 0;
        if (dnm){
            if (onLeft(L (q, ps[ret]), ps[mid]) > 0){
                ret = mid;
            }
        }
        if (dnl){
            if (onLeft(L (q, ps[ret]), ps[left]) > 0){
                ret = left;
            }
            if (dnm && onLeft(L (q, ps[left]), ps[mid]) > 0){
                right = mid - 1;
            }
            else{
                left = mid + 1;
            }
        }
        else{
            if (!dnm && onLeft(L (q, ps[left]), ps[mid]) > 0){
                left = mid + 1;
            }
            else{
                right = mid - 1;
            }
        }
    }
    return ret;
}

int tanConPL(std::vector <P> &ps, P q){
    int ret = 0, n = ps.size();
    int left = 1, right = n - 1;
    while (left <= right){
        bool dnl = onLeft(L (q, ps[left - 1]), ps[left]) < 0;
        int mid = left + right + 1 >> 1;
        bool dnm = onLeft(L (q, ps[mid - 1]), ps[mid]) < 0;
        if (dnm){
            if (onLeft(L (q, ps[ret]), ps[mid]) < 0){
                ret = mid;
            }
        }
        if (dnl){
            if (onLeft(L (q, ps[ret]), ps[left]) < 0){
                ret = left;
            }
            if (dnm && onLeft(L (q, ps[left]), ps[mid]) < 0){
                left = mid + 1;
            }
            else{
                right = mid - 1;
            }
        }
        else{
            if (!dnm && onLeft(L (q, ps[left]), ps[mid])){
                right = mid - 1;
            }
            else{
                left = mid + 1;
            }
        }
    }
    return ret;
}

// 要求凸包为逆时针方向
// 注意如果与某条边重合了，会出错
std::vector <P> isConL(std::vector <P> ps, P p, P q){
    std::vector <P> vec;
    int n = ps.size();
    for (int i = 0; i < n; ++ i){
        ps.push_back(ps[i]);
        vec.push_back(ps[i + 1] - ps[i]);
    }
    for (int i = 0; i < n; ++ i){
        vec.push_back(vec[i]);
    }
    for (int i = 0; i < n; ++ i){
        if (vec[i] < P{0, 0} && P{0, 0} < vec[i + 1]){
            std::rotate(ps.begin(), ps.begin() + i + 1, ps.end());
            std::rotate(vec.begin(), vec.begin() + i + 1, vec.end());
            break;
        }
    }
    int right = -1;
    for (int i = 0; i < n; ++ i){
        if (P{0, 0} < vec[i] && vec[i + 1] < P{0, 0}){
            right = i + 1;
            break;
        }
    }
    auto bin_search = [&](const std::vector <P> &src, P target, P delta, int l, int r){
        while (l < r){
            int mid = (l + r) / 2;
            if (sig((src[mid] - delta) ^ target) >= 0){
                l = mid + 1;
            }
            else{
                r = mid;
            }
        }
        return l;
    };
    if (q < p) std::swap(p, q);
    int low = bin_search(vec, q - p, P{0, 0}, 0, right) % n;
    int high = bin_search(vec, p - q, P{0, 0}, right, n) % n;
    int down = bin_search(ps, q - p, p, low, high + (low > high ? n : 0) + 1);
    int up = bin_search(ps, p - q, q, high, low + (high > low ? n : 0) + 1);
    if (down != low && up != high){
        down %= n, up %= n;
        L l(p, q);
        double t1 = isLL(l, L (ps[down - 1 + n], ps[down]));
        double t2 = isLL(l, L (ps[up - 1 + n], ps[up]));
        return {l.point(t1), l.point(t2)};
    }
    return {};
}

std::vector <P> cutCon(std::vector <P> &ps, P p, P q){//用pq的左半平面切割ps，O(n)
    std::vector <P> ret;
    int n = ps.size();
    for (int i = 0; i < n; ++ i){
        P a = ps[i], b = ps[(i + 1) % n];
        if (sig((q - p) ^ (a - p)) >= 0){
            ret.push_back(a);
        }
        if (!sig((q - p) ^ (b - a))){
            double t = isLL(L (a, b), L (p, q));
            if (sig(t) > 0 && sig(t - 1) < 0){
                ret.push_back(a + (b - a) * t);
            }
        }
    }
    return ret;
}

std::vector <P> isHalfPlane(std::vector <L> &ls){
    std::sort(ls.begin(), ls.end());
    std::deque <L> dq;
    std::deque <P> ps;
    dq.push_back(ls[0]);
    int n = ls.size();
    auto isLLP = [](L l1, L l2){return l1.point(isLL(l1, l2));};
    for (int i = 1; i < n; ++ i){
        while (!ps.empty() && sig(onLeft(ls[i], ps.back())) <= 0) ps.pop_back(), dq.pop_back();
        while (!ps.empty() && sig(onLeft(ls[i], ps.front())) <= 0) ps.pop_front(), dq.pop_front();
        L l = dq.back();
        if (sig(ls[i].v ^ l.v)){
            dq.push_back(ls[i]);
            ps.push_back(isLLP(l, ls[i]));
        }
        else{
            if (sig(onLeft(l, ls[i].p)) > 0){
                dq.pop_back();
                if (!dq.empty()){
                    ps.pop_back();
                    ps.push_back(isLLP(dq.back(), ls[i]));
                }
                dq.push_back(ls[i]);
            }
        }
    }
    while (!ps.empty() && sig(onLeft(dq.front(), ps.back())) <= 0) ps.pop_back(), dq.pop_back();
    if (dq.size() <= 2) return {};
    ps.push_back(isLLP(dq.front(), dq.back()));
    std::vector <P> ret;
    while (!ps.empty()){
        ret.push_back(ps.front());
        ps.pop_front();
    }
    return ret;
}

double areaCPol(C c, std::vector <P> &ps){
    auto area = [c](P p, P q){
        double theta = (p - c.o).angle() - (q - c.o).angle();
        if (theta < 0) theta += PI * 2;
        if (theta >= 2 * PI) theta -= PI * 2;
        theta = std::min(theta, 2 * PI - theta);
        return sqr(c.r) * theta;
    };
    double ret = 0;
    int n = ps.size();
    for (int i = 0; i < n; ++ i){
        P p1 = ps[i], p2 = ps[(i + 1) % n];
        L l(p1, p2);
        std::vector <double> vec = isCL(c, l);
        double tmp = 0;
        bool flag1 = sig((p1 - c.o).abs2() - sqr(c.r)) <= 0, flag2 = sig((p2 - c.o).abs2() - sqr(c.r)) <= 0;
        if (flag1 && flag2){
            tmp = std::abs((p1 - c.o) ^ (p2 - c.o));
        }
        else if (flag1 || flag2){
            if (!flag1){
                P p = l.point(vec[0]);
                tmp = area(p1, p) + std::abs((p - c.o) ^ (p2 - c.o));
            }
            else{
                P p = l.point(*(-- vec.end()));
                tmp = area(p, p2) + std::abs((p1 - c.o) ^ (p - c.o));
            }
        }
        else{
            if (vec.size() == 2 && sig(vec[0]) > 0 && sig(vec[0] - 1) < 0){
                P p3 = l.point(vec[0]), p4 = l.point(vec[1]);
                tmp = area(p1, p3) + area(p4, p2) + std::abs((p3 - c.o) ^ (p4 - c.o));
            }
            else{
                tmp = area(p1, p2);
            }
        }
        ret += tmp * sig((p1 - c.o) ^ (p2 - c.o));
    }
    return std::abs(ret) / 2;
}

std::pair <double, P> areaCs(std::vector <C> &cs){ // 圆并面积，重心，被卡时间时可预先去掉被其它圆包含的圆
    double ret = 0;
    P pret;
    int n = cs.size();
    for (int i = 0; i < n; ++ i){
        std::vector <std::pair <double, int>> vec = {{0, 0}, {PI * 2, 0}};
        int cnt = 1;
        for (int j = 0; j < n; ++ j){
            double dis = (cs[i].o - cs[j].o).abs();
            if (!sig(dis) && !sig(cs[i].r - cs[j].r)){
                if (i < j){
                    ++ cnt;
                }
                continue;
            }
            if (sig(dis - cs[j].r - cs[i].r) >= 0 || sig(dis + cs[j].r - cs[i].r) <= 0){
                continue;
            }
            if (sig(dis + cs[i].r - cs[j].r) <= 0){
                ++ cnt;
                continue;
            }
            double angle = L (cs[i].o, cs[j].o).angle;
            double p = (sqr(cs[i].r) + sqr(dis) - sqr(cs[j].r)) / (2 * cs[i].r * dis);
            p = std::acos(std::max(-1.0, std::min(1.0, p)));
            double left = angle - p, right = angle + p;
            if (left < 0) left += PI * 2;
            if (right < 0) right += PI * 2;
            if (left >= 2 * PI) left -= PI * 2;
            if (right >= 2 * PI) right -= PI * 2;
            vec.push_back({left, 1});
            vec.push_back({right, -1});
            if (left >= right){
                ++ cnt;
            }
        }
        std::sort(vec.begin(), vec.end());
        for (int j = 0, sz = vec.size(); j + 1 < sz; ++ j){
            cnt += vec[j].second;
            if (cnt == 1){
                double delta = vec[j + 1].first - vec[j].first;
                if (sig(delta) <= 0) continue;
                double sin = std::sin(delta / 2);
                P cen (4 * cs[i].r * sin * sin * sin / (3 * (delta - std::sin(delta))), 0);
                cen = cen.rot((vec[j].first + vec[j + 1].first) / 2) + cs[i].o;
                double area = sqr(cs[i].r) * (delta - std::sin(delta));
                pret = pret + cen * area;
                ret += area;
                P p1 = cs[i].point(vec[j].first), p2 = cs[i].point(vec[j + 1].first);
                area = p1 ^ p2;
                pret = pret + (p1 + p2) * area / 3;
                ret += area;
            }
        }
    }
    return {ret / 2, pret / ret};
}

double areaisCs(std::vector <C> &ccs){
    std::vector <C> cs;
    for (int i = 0, sz = ccs.size(); i < sz; ++ i){
        bool flag = true;
        for (int j = 0; j < sz; ++ j){
            if (i == j) continue;
            double dis = (ccs[i].o - ccs[j].o).abs();
            if (sig(dis - ccs[i].r - ccs[j].r) >= 0) return 0.0;
            if (!sig(dis) && !sig(ccs[i].r - ccs[j].r)){
                if (i < j){
                    flag = false;
                    break;
                }
            }
            else{
                if (sig(dis + ccs[i].r - ccs[j].r) <= 0){
                    flag = false;
                    break;
                }
            }
        }
        if (flag) cs.push_back(ccs[i]);
    }
    int n = cs.size();
    typedef std::pair <double, double> pdd;
    auto inter = [](const pdd &p1, const pdd &p2, std::vector <pdd> &vec){
        pdd p = {std::max(p1.first, p2.first), std::min(p1.second, p2.second)};
        if (p.first <= p.second) vec.push_back(p);
    };
    double ret = 0;
    for (int i = 0; i < n; ++ i){
        std::vector <pdd> vec = {{0, 2 * PI}};
        for (int j = 0; j < n; ++ j){
            double angle = L (cs[i].o, cs[j].o).angle;
            double dis = (cs[i].o - cs[j].o).abs();
            double p = (sqr(cs[i].r) + sqr(dis) - sqr(cs[j].r)) / (2 * cs[i].r * dis);
            p = std::acos(std::max(-1.0, std::min(1.0, p)));
            double left = angle - p, right = angle + p;
            if (left < 0) left += PI * 2;
            if (right < 0) right += PI * 2;
            if (left >= 2 * PI) left -= PI * 2;
            if (right >= 2 * PI) right -= PI * 2;
            std::vector <pdd> tmp;
            for (auto u : vec){
                if (left >= right){
                    inter({left, 2 * PI}, u, tmp);
                    inter({0, right}, u, tmp);
                }
                else{
                    inter({left, right}, u, tmp);
                }
            }
            vec = tmp;
        }
        for (auto u : vec){
            P p1 = cs[i].point(u.first), p2 = cs[i].point(u.second);
            double delta = u.second - u.first;
            ret += sqr(cs[i].r) * (delta - std::sin(delta));
            ret += p1 ^ p2;
        }
    }
    return ret / 2;
}

// 求点到凸多边形 ps 的距离，ps必须是逆时针的，p必须在ps外部
double disConvexP(std::vector <P> &ps, P q){
    int n = ps.size();
    int left = 0, right = n;
    auto pin = [](P p1, P p2, P q){
        return sig(p1 ^ q) >= 0 && sig(p2 ^ q) <= 0;
    };
    auto in = [pin](P p1, P p2, P p3, P p4, P q){
        P o12 = (p1 - p2).rot90();
        P o23 = (p2 - p3).rot90();
        P o34 = (p3 - p4).rot90();
        return pin(o12, o23, q - p2) || pin(o23, o34, q - p3)
               || pin(o23, p3 - p2, q - p2) && pin(p2 - p3, o23, q - p3);
    };
    while (right - left > 1){
        int mid = left + right >> 1;
        if (in(ps[(left + n - 1) % n], ps[left], ps[mid], ps[(mid + 1) % n], q)){
            right = mid;
        }
        else{
            left = mid;
        }
    }
    return disSP(ps[left], ps[right % n], q);
}

// 求凸多边形 ps 的直径
double convexDiameter(std::vector <P> &ps){
    int n = ps.size();
    int is = 0, js = 0;
    for (int i = 1; i < n; ++ i){
        if (ps[i].x > ps[is].x){
            is = i;
        }
        if (ps[i].x < ps[js].x){
            js = i;
        }
    }
    double maxd = (ps[is] - ps[js]).abs();
    int i = is, j = js;
    do{
        if (sig((ps[(i + 1) % n] - ps[i]) ^ (ps[(j + 1) % n] - ps[j])) >= 0){
            j = (j + 1) % n;
        }
        else{
            i = (i + 1) % n;
        }
        maxd = std::max(maxd, (ps[i] - ps[j]).abs());
    }
    while (i != is || j != js);
    return maxd;
}

// 求 ps 中的点的最小包围圆
C minCoverC(std::vector <P> &ps){
    int n = ps.size();
    std::random_shuffle(ps.begin(), ps.end());
    C ret(ps[0], 0);
    for (int i = 1; i < n; ++ i){
        if (sig((ps[i] - ret.o).abs() - ret.r) > 0){
            ret = C (ps[i], 0);
            for (int j = 0; j < i; ++ j){
                if (sig((ps[j] - ret.o).abs() - ret.r) > 0){
                    ret = C ((ps[i] + ps[j]) / 2, (ps[i] - ps[j]).abs() / 2);
                    for (int k = 0; k < j; ++ k){
                        if (sig((ps[k] - ret.o).abs() - ret.r) > 0){
                            ret = outC(ps[i], ps[j], ps[k]);
                        }
                    }
                }
            }
        }
    }
    return ret;
}

/*
void work() {
    scanf("%d" , &n);
    for (int i = 0 ; i < n ; ++ i) {
        L[i].input();
        P[i] = L[i];
    }
    int m = n;
    for (int i = 0 ; i + 1 < n ; ++ i)
        for (int j = i + 1 ; j + 1 < n ; ++ j) {
            if (dcmp((P[i + 1] - P[i]) ^ (P[j + 1] - P[j])) != 0)
                P[m ++] = GetLineIntersection(P[i] , P[i + 1] - P[i] , P[j] , P[j + 1] - P[j]);
        }
    sort(P , P + m);
    m = unique(P , P + m) - P;
    memset(pre , -1 , sizeof(pre));
    set< pair<int , int> > Hash;
    for (int i = 0 ; i + 1 < n ; ++ i) {
        vector< pair <Point , int> > V;
        for (int j = 0 ; j < m ; ++ j)
            if (OnSegment(P[j] , L[i] , L[i + 1]))
                V.push_back(make_pair(P[j] , j));
        sort(V.begin() , V.end());
        for (int j = 0 ; j + 1 < V.size() ; ++ j) {
            int x = V[j].second , y = V[j + 1].second;
            if (!Hash.count(make_pair(x , y))) {
                Hash.insert(make_pair(x , y));
                e[mcnt] = (edge) {y , pre[x]} , pre[x] = mcnt ++;
            }
            if (!Hash.count(make_pair(y , x))) {
                Hash.insert(make_pair(y , x));
                e[mcnt] = (edge) {x , pre[y]} , pre[y] = mcnt ++;
            }
        }
    }
    for (int x = 0 ; x < m ; ++ x) {
        vector< pair<double , int> > V;
        for (int i = pre[x] ; ~i ; i = e[i].next) {
            int y = e[i].x;
            V.push_back(make_pair((P[y] - P[x]).arg() , i));
        }
        sort(V.begin() , V.end());
        for (int i = 0 ; i < V.size() ; ++ i) {
            int j = (i + 1) % V.size();
            Next[V[j].second ^ 1] = V[i].second;
        }
    }
    double res = 0;
    for (int i = 0 ; i < mcnt ; ++ i) {
        if (!vis[i]) {
            int x = i;
            double area = 0;
            while (!vis[x]) {
                vis[x] = 1;
                area += (P[e[x ^ 1].x] ^ P[e[x].x]);
                x = Next[x];
            }
            if (x == i && dcmp(area) > 0)
                res += area;
        }
    }
    printf("%.8f\n" , res / 2);
}
*/

int main(){
    srand((unsigned) time(NULL));
    return 0;
}
