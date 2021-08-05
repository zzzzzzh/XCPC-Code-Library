<TeX>
可并堆优化的最小树形图，时间复杂度 $\mathcal{O}(m\log n)$。支持输出方案。
若输入不保证强连通，需要先给所有 $i\to i+1$ 及 $n\to1$ 连上 $+\infty$ 的边，使得图强连通
</TeX>
#include <bits/stdc++.h>
#define ls(u) (ch[(u)][0])
#define rs(u) (ch[(u)][1])
using ll = long long;
using pii = std::pair <ll, int>;

namespace LH{
    const static int N = 200010;
int sz;
    pii value[N];
    ll w0[N], lazy[N];
    int dis[N];
    int ch[N][2];

    int newnode(pii val){
        ++ sz;
        value[sz] = val;
        lazy[sz] = 0;
        dis[sz] = 0;
        memset(ch[sz], 0, sizeof(ch[0]));
        return sz;
    }

    void update(int pos, int a){
        if (!pos) return;
        lazy[pos] += a;
        value[pos].first += a;
        w0[value[pos].second] += a;
    }

    void push(int u){
        if (!lazy[u]) return;
        update(ls(u), lazy[u]);
        update(rs(u), lazy[u]);
        lazy[u] = 0;
    }

    void init(){
        sz = 0;
    }

    int merge(int u, int v){
        if (!u || !v) return u ^ v;
        if (value[u] > value[v]){
            std::swap(u, v);
        }
        push(u);
        rs(u) = merge(rs(u), v);
        if (dis[rs(u)] > dis[ls(u)]){
            std::swap(ls(u), rs(u));
        }
        dis[u] = dis[rs(u)] + 1;
        return u;
    }

    struct LefistHeap{
        int root;

        explicit LefistHeap():root(0){}

        void init(pii val){
            root = newnode(val);
        }

        pii top(){
            return value[root];
        }

        void push(pii val){
            int nd = newnode(val);
            root = merge(root, nd);
        }

        void pop(){
            root = merge(ls(root), rs(root));
        }

        void join(LefistHeap lh){
            root = merge(root, lh.root);
        }

        bool empty(){
            return root == 0;
        }

        void add(ll a){
            update(root, a);
        }
    };
}

namespace MDST{ // Do not forget to make the graph strong-connected!!!
    const int N = 200010;
    int in[N], prev[N], parent[N], fa[N];
    std::vector <int> e[N];
    std::vector <int> children[N];
    LH::LefistHeap p[N];
    int u[N], v[N];
    ll w[N];
    int sz, ecnt;

    int find(int u){
        return fa[u] == u ? u : (fa[u] = find(fa[u]));
    }

    int newnode(){
        ++ sz;
        in[sz] = prev[sz] = parent[sz] = 0;
        fa[sz] = sz;
        children[sz].clear();
        p[sz] = LH::LefistHeap();
        return sz;
    }

    void addedge(int uu, int vv, ll ww){
        ++ ecnt;
        u[ecnt] = uu, v[ecnt] = vv, w[ecnt] = LH::w0[ecnt] = ww;
        e[uu].emplace_back(ecnt);
    }

    void init(int n){
        LH::init();
        sz = 0;
        for (int i = 1; i <= n; ++ i){
            newnode();
        }
        for (int i = 1; i <= n; ++ i){
            for (auto id : e[i]){
                p[v[id]].push({w[id], id});
            }
        }
    }

    void contract(int n){
        init(n);
        int a = 1;
        while (!p[a].empty()){
            int id = p[a].top().second;
            p[a].pop();
            int b = find(u[id]);
            if (a == b){
                continue;
            }
            in[a] = id;
            prev[a] = b;
            if (in[b] == 0){
                a = b;
                continue;
            }
            int c = newnode();
            while (a != c){
                parent[a] = fa[a] = c;
                p[a].add(-LH::w0[in[a]]);
                children[c].emplace_back(a);
                p[c].join(p[a]);
                a = find(prev[a]);
            }
        }
    }

    int in_res[N], parent_back[N];

    void dismantle(int u, std::vector <int> &vec){
        while (parent_back[u]){
            for (auto v : children[parent_back[u]]){
                if (v == u){
                    continue;
                }
                parent_back[v] = 0;
                vec.emplace_back(v);
            }
            u = parent_back[u];
        }
    }

    void expand(int rt){ // by call expand, solution stored in in_res
        memcpy(parent_back, parent, sizeof(parent[0]) * (sz + 1));
        std::vector <int> vec;
        dismantle(rt, vec);
        while (!vec.empty()){
            int c = vec.back();
            vec.pop_back();
            int id = in[c];
            in_res[v[id]] = id;
            dismantle(v[id], vec);
        }
    }

    void clear(int n){
        for (int i = 1; i <= n; ++ i){
            e[i].clear();
        }
        ecnt = 0;
    }
}
