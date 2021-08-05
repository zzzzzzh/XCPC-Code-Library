#include <bits/stdc++.h>
#define ls(u) (ch[(u)][0])
#define rs(u) (ch[(u)][1])

const int N = 200010;
const int M = 10000010;

std::vector <int> e[N];
int father[N], sz[N];
int value[N];
bool vis[N];
int dfs_rt, min_value;
std::vector <int> ancestor[N];

void get_sz(int u, int fa){
    sz[u] = 1;
    for (auto v : e[u]){
        if (vis[v] || v == fa){
            continue;
        }
        get_sz(v, u);
        sz[u] += sz[v];
    }
}

void get_rt(int u, int fa, int tot){
    int max = 0;
    for (auto v : e[u]){
        if (vis[v] || v == fa){
            continue;
        }
        get_rt(v, u, tot);
        max = std::max(max, sz[v]);
    }
    max = std::max(max, tot - sz[u]);
    if (max < min_value){
        min_value = max;
        dfs_rt = u;
    }
}

int dfs(int u, int prev){
    get_sz(u, 0);
    min_value = INT_MAX;
    get_rt(u, 0, sz[u]);
    int root = dfs_rt;
    ancestor[root] = ancestor[prev];
    ancestor[root].emplace_back(root);
    vis[root] = true;
    for (auto v : e[root]){
        if (vis[v]){
            continue;
        }
        int nroot = dfs(v, root);
        father[nroot] = root;
    }
    return root;
}

namespace LCA{ // N should >= 2n
    const int BIT = 18;
    int euler[N], euler_cnt;
    int dep[N], left[N], right[N];
    int lb[N], st[N][BIT];

    void dfs(int u, int fa){
        left[u] = euler_cnt;
        euler[euler_cnt ++] = u;
        for (auto v : e[u]){
            if (v == fa){
                continue;
            }
            dep[v] = dep[u] + 1;
            dfs(v, u);
            right[u] = euler_cnt;
            euler[euler_cnt ++] = u;
        }
    }

    int lca(int u, int v){
        int x = left[u], y = left[v];
        if (x > y) std::swap(x, y);
        int bit = lb[y - x + 1];
        u = st[x][bit], v = st[y - (1 << bit) + 1][bit];
        return dep[u] < dep[v] ? u : v;
    }

    void process(){
        for (int i = 2; i < N; ++ i){
            lb[i] = lb[i >> 1] + 1;
        }
        dfs(1, 0);
        for (int i = 0; i < euler_cnt; ++ i){
            st[i][0] = euler[i];
        }
        for (int j = 1; j < BIT; ++ j){
            for (int i = 0; i < euler_cnt; ++ i){
                int next = i + (1 << (j - 1));
                if (next >= euler_cnt){
                    break;
                }
                int x = st[i][j - 1], y = st[next][j - 1];
                st[i][j] = dep[x] < dep[y] ? x : y;
            }
        }
    }

    int dis(int u, int v){
        int l = lca(u, v);
        return dep[u] + dep[v] - 2 * dep[l];
    }
}

int seg_sz;
int rt[N][2], seg[M], ch[M][2];

int newnode(){
    return ++ seg_sz;
}

void add(int &sit, int l, int r, int q, int value){
    if (q > r || q < l){
        return;
    }
    if (!sit){
        sit = newnode();
    }
    seg[sit] += value;
    if (l == r){
        return;
    }
    int mid = (l + r) / 2;
    add(ls(sit), l, mid, q, value);
    add(rs(sit), mid + 1, r, q, value);
}

int query(int sit, int l, int r, int ql, int qr){
    if (!sit || ql > r || qr < l){
        return 0;
    }
    if (ql <= l && r <= qr){
        return seg[sit];
    }
    int mid = (l + r) / 2;
    return query(ls(sit), l, mid, ql, qr) + query(rs(sit), mid + 1, r, ql, qr);
}

void update(int x, int value, int n){
    int sz = ancestor[x].size();
    for (int i = 0; i < sz; ++ i){
        int fa = ancestor[x][i];
        int dis = LCA::dis(fa, x);
        add(rt[fa][0], 0, n, dis, value);
        if (i < sz - 1){
            add(rt[ancestor[x][i + 1]][1], 0, n, dis, value);
        }
    }
}

int main(){
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++ i){
        scanf("%d", &value[i]);
    }
    for (int i = 0, u, v; i < n - 1; ++ i){
        scanf("%d%d", &u, &v);
        e[u].emplace_back(v);
        e[v].emplace_back(u);
    }
    LCA::process();
    dfs(1, 0);
    for (int i = 1; i <= n; ++ i){
        update(i, value[i], n);
    }
    int last = 0;
    while (m --){
        int type;
        scanf("%d", &type);
        if (type == 0){
            int x, k;
            scanf("%d%d", &x, &k);
            x ^= last, k ^= last;
            int sz = ancestor[x].size();
            last = 0;
            for (int i = 0; i < sz; ++ i){
                int fa = ancestor[x][i];
                int dis = LCA::dis(fa, x);
                last += query(rt[fa][0], 0, n, 0, k - dis);
                if (i < sz - 1){
                    last -= query(rt[ancestor[x][i + 1]][1], 0, n, 0, k - dis);
                }
            }
            printf("%d\n", last);
        }
        else{
            int x, y;
            scanf("%d%d", &x, &y);
            x ^= last, y ^= last;
            update(x, y - value[x], n);
            value[x] = y;
        }
    }
    return 0;
}
