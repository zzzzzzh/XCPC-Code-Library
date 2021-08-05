#include<bits/stdc++.h>

using ll = long long;
const ll INF = ll(7.1e13);
const int DIM = 2;

using Node = std::array <int, DIM>;

struct KD_Tree{
#define ls(u) (ch[(u)][0])
#define rs(u) (ch[(u)][1])
    const static int N = 300010;

    Node low[N], high[N];
    ll seg[N], seg1[N], lazy[N], pre[N]; // seg1: history min
    int ch[N][2], sz, root;
    std::vector<Node> vec;

    int newnode(){
        ++ sz;
        memset(ch[sz], 0, sizeof(ch[0]));
        lazy[sz] = pre[sz] = 0;
        return sz;
    }

    void init(){
        sz = 0;
        root = newnode();
    }

    void pull(int u){
        for (int i = 0; i < DIM; ++ i){
            low[u][i] = std::min(low[ls(u)][i], low[rs(u)][i]);
            high[u][i] = std::max(high[ls(u)][i], high[rs(u)][i]);
        }
        pull1(u);
    }

    void pull1(int u){
        seg[u] = std::min(seg[ls(u)], seg[rs(u)]);
        seg1[u] = std::min(seg1[ls(u)], seg1[rs(u)]);
    }

    void build(int u, int l, int r, int dep){ // ensure vec assigned and non-empty!
        if (l == r){
            low[u] = high[u] = vec[l];
            ll calc_length(Node node);
            seg[u] = seg1[u] = calc_length(vec[l]);
            return;
        }
        int mid = (l + r) / 2;
        std::nth_element(vec.begin() + l, vec.begin() + mid, vec.begin() + r + 1,
                         [&](const Node &n1, const Node &n2){
                             return n1[dep] < n2[dep];
        });
        ls(u) = newnode();
        rs(u) = newnode();
        build(ls(u), l, mid, (dep + 1) % DIM);
        build(rs(u), mid + 1, r, (dep + 1) % DIM);
        pull(u);
    }

    void build(){
        if (vec.empty()) return;
        build(root, 0, int(vec.size()) - 1, 0);
    }

    void push(int sit){
        if (!lazy[sit] && !pre[sit]){
            return;
        }
        seg1[ls(sit)] = std::min(seg1[ls(sit)], seg[ls(sit)] + pre[sit]);
        seg1[rs(sit)] = std::min(seg1[rs(sit)], seg[rs(sit)] + pre[sit]);
        pre[ls(sit)] = std::min(pre[ls(sit)], pre[sit] + lazy[ls(sit)]);
        pre[rs(sit)] = std::min(pre[rs(sit)], pre[sit] + lazy[rs(sit)]);
        seg[ls(sit)] += lazy[sit];
        seg[rs(sit)] += lazy[sit];
        lazy[ls(sit)] += lazy[sit];
        lazy[rs(sit)] += lazy[sit];
        lazy[sit] = pre[sit] = 0;
    }

    void add(int sit, Node ql, Node qr, ll value){
        if (!sit) return;
        for (int i = 0; i < DIM; ++ i){
            if (qr[i] < low[sit][i] || ql[i] > high[sit][i]) return;
        }
        for (int i = 0; i < DIM; ++ i){
            if (!(ql[i] <= low[sit][i] && high[sit][i] <= qr[i])){
                push(sit);
                add(ls(sit), ql, qr, value);
                add(rs(sit), ql, qr, value);
                pull1(sit);
                return;
            }
        }
        seg[sit] += value;
        seg1[sit] = std::min(seg1[sit], seg[sit]);
        lazy[sit] += value;
        pre[sit] = std::min(pre[sit], lazy[sit]);
    }

    ll query(int sit, Node ql, Node qr){
        if (!sit) return LLONG_MAX;
        for (int i = 0; i < DIM; ++ i){
            if (qr[i] < low[sit][i] || ql[i] > high[sit][i]) return LLONG_MAX;
        }
        for (int i = 0; i < DIM; ++ i){
            if (!(ql[i] <= low[sit][i] && high[sit][i] <= qr[i])){
                push(sit);
                return std::min(query(ls(sit), ql, qr), query(rs(sit), ql, qr));
            }
        }
        return seg1[sit];
    }
};

const int N = 150010;
const int BIT = 18;

std::vector<int> e[N];
int u[N], v[N], w1[N], w2[N], x1[N], x2[N];
int dep[N], fafa[N][BIT], dfn[N], right[N], dfn_cnt, id[N];
int qu[N], qv[N];

void dfs(int u, int fa){
    fafa[u][0] = fa;
    for (int i = 1; i < BIT; ++ i){
        fafa[u][i] = fafa[fafa[u][i - 1]][i - 1];
    }
    dfn[u] = ++ dfn_cnt;
    id[dfn_cnt] = u;
    for (auto v : e[u]){
        if (v == fa){
            continue;
        }
        dep[v] = dep[u] + 1;
        dfs(v, u);
    }
    right[u] = dfn_cnt;
}

int lca(int u, int v){
    if (dep[u] < dep[v]){
        std::swap(u, v);
    }
    for (int i = 0; i < BIT; ++ i){
        if ((dep[u] - dep[v]) >> i & 1){
            u = fafa[u][i];
        }
    }
    if (u == v){
        return u;
    }
    for (int i = BIT - 1; i >= 0; -- i){
        if (fafa[u][i] != fafa[v][i]){
            u = fafa[u][i], v = fafa[v][i];
        }
    }
    return fafa[u][0];
}

ll calc_length(Node node){
    int uu = id[node[0]], vv = id[node[1]];
    return (dep[uu] + dep[vv] - 2 * dep[lca(uu, vv)]) * INF;
}

KD_Tree kd_tree;
ll cur[N];

int main(){
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n - 1; ++ i){
        scanf("%d%d%d%d%d%d", &u[i], &v[i], &w1[i], &w2[i], &x1[i], &x2[i]);
        e[u[i]].emplace_back(v[i]);
        e[v[i]].emplace_back(u[i]);
    }
    int root = 1;
    dfs(root, 0);
    std::vector<Node> node;
    for (int i = 0; i < m; ++ i){
        scanf("%d%d", &qu[i], &qv[i]);
        qu[i] = dfn[qu[i]];
        qv[i] = dfn[qv[i]];
        if (qu[i] > qv[i]){
            std::swap(qu[i], qv[i]);
        }
        node.push_back({qu[i], qv[i]});
    }
    std::sort(node.begin(), node.end());
    node.erase(std::unique(node.begin(), node.end()), node.end());
    kd_tree.init();
    kd_tree.vec = node;
    kd_tree.build();
    using pii = std::pair <int, int>;
    std::map<int, std::vector<pii>> map;
    for (int i = 0; i < n - 1; ++ i){
        int down = dep[u[i]] > dep[v[i]] ? u[i] : v[i];
        map[x1[i]].emplace_back(w1[i], down);
        map[x2[i]].emplace_back(w2[i], down);
    }
    int pre = 0;
    for (int i = 1; i <= n; ++ i){
        cur[i] = INF;
    }
    for (const auto &pair : map){
        kd_tree.add(kd_tree.root, {1, 1}, {n, n}, pair.first - pre);
        pre = pair.first;
        for (const auto &p : pair.second){
            if (p.first < cur[p.second]){
                ll diff = p.first - cur[p.second];
                cur[p.second] = p.first;
                int l = dfn[p.second], r = right[p.second];
                if (l > 1){
                    kd_tree.add(kd_tree.root, {1, l}, {l - 1, r}, diff);
                }
                if (r < n){
                    kd_tree.add(kd_tree.root, {l, r + 1}, {r, n}, diff);
                }
            }
        }
    }
    for (int i = 0; i < m; ++ i){
        Node nnode = {qu[i], qv[i]};
        printf("%lld\n", kd_tree.query(kd_tree.root, nnode, nnode));
    }
    return 0;
}