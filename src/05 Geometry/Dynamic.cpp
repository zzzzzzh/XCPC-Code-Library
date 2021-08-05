<TeX>不知道什么题的代码</TeX>
#include <bits/stdc++.h>

using LL = long long;

const int max_N = 1000 + 21;

struct P {
    int x, y;

    explicit P(int x = 0, int y = 0) : x(x), y(y) {}

    inline P add(const P &p) const { return P(x + p.x, y + p.y); }

    inline P sub(const P &p) const { return P(x - p.x, y - p.y); }

    inline LL dot(const P &p) const { return 1ll * x * p.x + 1ll * y * p.y; }

    inline LL det(const P &p) const { return 1ll * x * p.y - 1ll * y * p.x; }

    inline LL abs2() { return dot(*this); }

    inline bool operator==(const P &p) { return x == p.x && y == p.y; }
} p[max_N], s[max_N], cur;

int n, c[max_N], Q[max_N], rk[max_N];

LL ans[max_N];

bool fir[max_N], vis[max_N];

struct functor {
    bool operator()(int a, int b) {
        P x = p[Q[a + 1]].sub(p[Q[a]]);
        P y = p[Q[b + 1]].sub(p[Q[b]]);
        LL det = x.det(y);
        if (det) return det > 0;
        if (x.dot(y) < 0) return cur.dot(x) > 0;
        return a < b;
    }
};

std::set<int, functor> set;

void updt(int i) {
    s[i] = s[i - 1];
    rk[i] = rk[i - 1];
    int x = Q[i];
    if (fir[x]) {
        s[i] = s[i].add(p[x]);
        ++rk[i];
        ans[rk[i]] = std::max(ans[rk[i]], s[i].abs2());
    }
}

void maintain(int i, bool tp) {
    if (!i || i >= n) return;
    if (p[Q[i]] == p[Q[i + 1]]) return;
    if (tp) set.insert(i);
    else set.erase(i);
}

void swap(int i) {
    maintain(i - 1, false);
    maintain(i, false);
    maintain(i + 1, false);
    std::swap(Q[i], Q[i + 1]);
    if (c[Q[i]] == c[Q[i + 1]] && (fir[Q[i]] || fir[Q[i + 1]])) {
        std::swap(fir[Q[i]], fir[Q[i + 1]]);
        for (int j = i; j <= n; ++j) updt(j);
    } else {
        updt(i), updt(i + 1);
    }
    maintain(i - 1, true);
    maintain(i, true);
    maintain(i + 1, true);
}

void solve() {
    for (int i = 1; i <= n; ++i) Q[i] = i;
    std::sort(Q + 1, Q + 1 + n, [&](int a, int b) {
        return p[a].y == p[b].y ? p[a].x < p[b].x : p[a].y < p[b].y;
    });
    cur = P(1, 0);
    for (int i = 1; i <= n; ++i) {
        if (!vis[c[Q[i]]]) vis[c[Q[i]]] = fir[Q[i]] = true;
        updt(i);
        maintain(i, true);
    }
    P st = cur, pre = cur;
    while (!set.empty()) {
        int x = *set.begin();
        set.erase(set.begin());
        cur = p[Q[x + 1]].sub(p[Q[x]]);
        if ((pre.det(st) > 0 || (!pre.det(st) && pre.dot(st) < 0)) &&
            (st.det(cur) > 0 || (!st.det(cur) && st.dot(cur) > 0)))
            break;
        swap(x);
        pre = cur;
    }
}

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; ++i) {
        scanf("%d%d%d", &p[i].x, &p[i].y, c + i);
    }
    solve();
    for (int i = 1; i <= n; ++i) {
        ans[i] = std::max(ans[i], ans[i - 1]);
        printf("%lld%c", ans[i], " \n"[i == n]);
    }
    return 0;
}
