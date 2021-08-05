#include <bits/stdc++.h>

using pii = std::pair <int, int>;
const int N = 100010;
std::mt19937 rnd(19971109);

struct Treap{
#define ls(u) (ch[(u)][0])
#define rs(u) (ch[(u)][1])

    int root, node_sz;
    int ch[N][2];
    int key[N], sz[N], cnt[N];
    unsigned prior[N];

    void clear(int x){
        ls(x) = rs(x) = 0;
        key[x] = sz[x] = cnt[x] = 0;
    }

    void init(){
        node_sz = 0;
    }

    int newnode(int x){
        clear(++ node_sz);
        key[node_sz] = x;
        prior[node_sz] = rnd();
        return node_sz;
    }

    void pull(int u){
        sz[u] = sz[ls(u)] + sz[rs(u)] + cnt[u];
    }

    pii split(int p, int u){
        if (!p) return {0, 0};
        if (key[p] <= u){
            pii ret = split(rs(p), u);
            rs(p) = ret.first;
            pull(p);
            return {p, ret.second};
        }
        pii ret = split(ls(p), u);
        ls(p) = ret.second;
        pull(p);
        return {ret.first, p};
    }

    int merge(int u, int v){
        if (!u || !v) return u ^ v;
        if (prior[u] > prior[v]){
            rs(u) = merge(rs(u), v);
            pull(u);
            return u;
        }
        ls(v) = merge(u, ls(v));
        pull(v);
        return v;
    }

    int right(int p){
        while (rs(p)) p = rs(p);
        return p;
    }

    void insert(int x){
        pii pair = split(root, x);
        int p = right(pair.first);
        pii pair1;
        if (p && key[p] == x){
            pair1 = split(pair.first, x - 1);
        }
        else{
            pair1 = {pair.first, newnode(x)};
        }
        ++ cnt[pair1.second], ++ sz[pair1.second];
        root = merge(pair1.first, pair1.second);
        root = merge(root, pair.second);
    }

    void erase(int x){
        pii pair = split(root, x);
        pii pair1 = split(pair.first, x - 1);
        if (!pair1.second || cnt[pair1.second] == 1){
            root = merge(pair1.first, pair.second);
            return;
        }
        -- cnt[pair1.second], -- sz[pair1.second];
        root = merge(pair1.first, pair1.second);
        root = merge(root, pair.second);
    }

    int rk(int u, int x){
        if (!u) return 0;
        if (key[u] < x) return sz[ls(u)] + cnt[u] + rk(rs(u), x);
        return rk(ls(u), x);
    }

    int query(int u, int k){
        if (k <= sz[ls(u)]) return query(ls(u), k);
        if (k <= sz[ls(u)] + cnt[u]) return key[u];
        return query(rs(u), k - sz[ls(u)] - cnt[u]);
    }

    int prev(int u, int x){
        if (!u) return INT_MIN;
        if (x <= key[u]) return prev(ls(u), x);
        int ret = prev(rs(u), x);
        return ret == INT_MIN ? key[u] : ret;
    }

    int next(int u, int x){
        if (!u) return INT_MAX;
        if (x >= key[u]) return next(rs(u), x);
        int ret = next(ls(u), x);
        return ret == INT_MAX ? key[u] : ret;
    }
};

Treap treap;

int main(){
    int n;
    scanf("%d", &n);
    while (n --){
        int op, x;
        scanf("%d%d", &op, &x);
        switch (op) {
            case 1: treap.insert(x); break;
            case 2: treap.erase(x); break;
            case 3: printf("%d\n", treap.rk(treap.root, x) + 1); break;
            case 4: printf("%d\n", treap.query(treap.root, x)); break;
            case 5: printf("%d\n", treap.prev(treap.root, x)); break;
            case 6: printf("%d\n", treap.next(treap.root, x)); break;
        }
    }
    return 0;
}
