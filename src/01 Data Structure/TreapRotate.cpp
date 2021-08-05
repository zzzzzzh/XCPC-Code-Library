#include <bits/stdc++.h>

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

    int newnode(){
        clear(++ node_sz);
        prior[node_sz] = rnd();
        return node_sz;
    }

    void pull(int u){
        sz[u] = sz[ls(u)] + sz[rs(u)] + cnt[u];
    }

    void lrotate(int &p){
        int q = rs(p);
        rs(p) = ls(q);
        ls(q) = p;
        sz[q] = sz[p];
        pull(p);
        p = q;
    }

    void rrotate(int &p){
        int q = ls(p);
        ls(p) = rs(q);
        rs(q) = p;
        sz[q] = sz[p];
        pull(p);
        p = q;
    }

    void insert(int &u, int x){
        if (!u){
            u = newnode();
            key[u] = x;
            cnt[u] = sz[u] = 1;
            return;
        }
        ++ sz[u];
        if (key[u] == x){
            ++ cnt[u];
            return;
        }
        if (key[u] < x){
            insert(rs(u), x);
            if (prior[rs(u)] > prior[u]) lrotate(u);
        }
        else{
            insert(ls(u), x);
            if (prior[ls(u)] > prior[u]) rrotate(u);
        }
    }

    void erase(int &u, int x){
        if (!u) return;
        if (key[u] == x){
            if (cnt[u] > 1){
                sz[u] -= 1, cnt[u] -= 1;
                return;
            }
            if (!ls(u) || !rs(u)){
                u = ls(u) ^ rs(u);
                return;
            }
            prior[ls(u)] < prior[rs(u)] ? lrotate(u) : rrotate(u);
            erase(u, x);
            return;
        }
        -- sz[u];
        erase(ch[u][key[u] < x], x);
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
    treap.init();
    while (n --){
        int op, x;
        scanf("%d%d", &op, &x);
        switch (op) {
            case 1: treap.insert(treap.root, x); break;
            case 2: treap.erase(treap.root, x); break;
            case 3: printf("%d\n", treap.rk(treap.root, x) + 1); break;
            case 4: printf("%d\n", treap.query(treap.root, x)); break;
            case 5: printf("%d\n", treap.prev(treap.root, x)); break;
            case 6: printf("%d\n", treap.next(treap.root, x)); break;
        }
    }
    return 0;
}
