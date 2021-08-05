<TeX>不知道什么题的代码...</TeX>
#include <bits/stdc++.h>

using Float = long double;

const int max_N = (int) 1e6 + 21;

const Float eps = 1e-6;

inline int dcmp(Float x) {
    if (x < -eps) return -1;
    if (x > +eps) return +1;
    return 0;
}

inline Float sqr(Float x) {
    return x * x;
}

struct poi {
    Float x, y;

    explicit poi(Float x = 0., Float y = 0.) : x(x), y(y) {}

    inline poi add(const poi &p) const {
        return poi(x + p.x, y + p.y);
    }

    inline poi sub(const poi &p) const {
        return poi(x - p.x, y - p.y);
    }

    inline poi mul(Float k) const {
        return poi(x * k, y * k);
    }

    inline Float dot(const poi &p) const {
        return x * p.x + y * p.y;
    }

    inline Float det(const poi &p) const {
        return x * p.y - y * p.x;
    }

    inline Float abs2() const {
        return dot(*this);
    }
} p[max_N];

inline Float dis2(const poi &p1, const poi &p2) {
    return p2.sub(p1).abs2();
}

using cir = std::pair<poi, Float>; // o, r^2

inline cir cir2(const poi &p1, const poi &p2) {
    poi o = p1.add(p2).mul(0.5);
    Float r2 = dis2(p1, p2) * 0.25;
    return {o, r2};
}

inline bool out(const cir &c, const poi &p) {
    return dcmp(dis2(c.first, p) - c.second) > 0;
}

int n;

#define SZ(x) int((x).size())

inline cir cir3(std::vector<int> &vec) {
    Float q = 0;
    for (int i = 0; i < 3; ++i) {
        int j = i == 2 ? 0 : i + 1;
        q += p[vec[i]].det(p[vec[j]]);
    }
    if (!dcmp(q)) {
        cir c = {poi(), -1};
        for (int i = 0; i < 3; ++i)
            for (int j = i + 1; j < 3; ++j) {
                double r2 = dis2(p[vec[i]], p[vec[j]]) * 0.25;
                if (r2 > c.second) {
                    c = {p[vec[i]].add(p[vec[j]]).mul(0.5), r2};
                }
            }
        return c;
    }
    std::vector<Float> abs2;
    for (auto &i : vec) {
        abs2.push_back(p[i].abs2());
    }
    Float x = 0, y = 0;
    for (int i = 0; i < 3; ++i) {
        int j = i == 2 ? 0 : i + 1;
        x += poi(abs2[i], abs2[j]).det(poi(p[vec[i]].y, p[vec[j]].y));
        y -= poi(abs2[i], abs2[j]).det(poi(p[vec[i]].x, p[vec[j]].x));
    }
    poi o = poi(x, y).mul(0.5 / q);
    return {o, dis2(o, p[vec[0]])};
}

void solve() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(p + 1, p + 1 + n, std::default_random_engine(seed));
    cir c = cir2(p[1], p[2]);
    for (int i = 3; i <= n; ++i) {
        if (!out(c, p[i])) continue;
        c = cir2(p[1], p[i]);
        for (int j = 1; j < i; ++j) {
            if (!out(c, p[j])) continue;
            c = cir2(p[i], p[j]);
            for (int k = 1; k < j; ++k) {
                if (!out(c, p[k])) continue;
                std::vector<int> vec = {i, j, k};
                c = cir3(vec);
            }
        }
    }
    double x = c.first.x, y = c.first.y;
    double r = std::sqrt(c.second);
    printf("%.2f %.2f %.2f\n", x, y, r);
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        double x, y;
        scanf("%lf%lf", &x, &y);
        p[i] = poi(x, y);
    }
    solve();
    return 0;
}
