<TeX>不知道什么题的源代码...</TeX>
#include <bits/stdc++.h>

#define ALL(x) (x).begin(), (x).end()

using LL = long long;
using PII = std::pair<int, int>;

const int max_N = 2000 + 21;

struct P {
    int x, y;

    explicit P(int x = 0, int y = 0) : x(x), y(y) {}

    inline P sub(const P &p) const { return P(x - p.x, y - p.y); }

    inline LL dot(const P &p) const { return 1ll * x * p.x + 1ll * y * p.y; }

    inline LL det(const P &p) const { return 1ll * x * p.y - 1ll * y * p.x; }

    inline P rot90() const { return P(-y, x); }
} p[max_N], v[max_N][max_N];

int n, Q[max_N], rk[max_N];

LL S;

std::vector<PII> vec;

LL area(int i, int j, int k) { return p[i].sub(p[k]).det(p[j].sub(p[k])); }

struct functor {
    inline bool operator()(const PII &a, const PII &b) const {
        return v[a.first][a.second].det(v[b.first][b.second]) > 0;
    }
};

void binarySearch(int u, int v) {
    int lo = 1, hi = n;
    while (lo <= hi) {
        int mi = (lo + hi) >> 1;
        LL s = area(u, v, Q[mi]);
        if (s == S) {
            puts("Yes");
            printf("%d %d\n", p[u].x, p[u].y);
            printf("%d %d\n", p[v].x, p[v].y);
            printf("%d %d\n", p[Q[mi]].x, p[Q[mi]].y);
            exit(0);
        }
        if (s < S) lo = mi + 1;
        else hi = mi - 1;
    }
}

void solve() {
    std::sort(p + 1, p + 1 + n, [&](P a, P b) { return a.y == b.y ? a.x < b.x : a.y < b.y; });
    for (int i = 1; i <= n; ++i)
        for (int j = i + 1; j <= n; ++j) {
            v[i][j] = p[j].sub(p[i]);
            vec.push_back({i, j});
        }
    std::sort(ALL(vec), functor());
    for (int i = 1; i <= n; ++i) Q[i] = rk[i] = i;
    for (int i = 1; i <= n; ++i) rk[Q[i]] = i;
    for (int i = 0, sz = int(vec.size()); i < sz; ++i) {
        int u = vec[i].first, v = vec[i].second;
        binarySearch(u, v);
        binarySearch(v, u);
        std::swap(Q[rk[u]], Q[rk[v]]);
        std::swap(rk[u], rk[v]);
    }
    puts("No");
}

int main() {
    scanf("%d%lld", &n, &S);
    S <<= 1;
    for (int i = 1; i <= n; ++i) {
        scanf("%d%d", &p[i].x, &p[i].y);
    }
    solve();
    return 0;
}
