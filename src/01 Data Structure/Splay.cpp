#include <bits/stdc++.h>

const int N = 1000010;

struct Splay{ // 注意 prev next 等操作带有 insert，算结点数的时候要算进去
#define ls(u) (ch[(u)][0])
#define rs(u) (ch[(u)][1])

    int root, node_sz;
    int ch[N][2], fa[N];
    int key[N], cnt[N], sz[N];

    void clear(int x){
        ls(x) = rs(x) = fa[x] = 0;
        key[x] = cnt[x] = sz[x] = 0;
    }

    void init(){
        node_sz = 0;
    }

    int newnode(){
        clear(++ node_sz);
        return node_sz;
    }

    void pull(int u){
        sz[u] = sz[ls(u)] + sz[rs(u)] + cnt[u];
    }

    int get(int x){
        return x == rs(fa[x]);
    }

    void rotate(int x){
        int y = fa[x], z = fa[y], sit = get(x);
        if (z) ch[z][get(y)] = x;
        int u = ch[x][sit ^ 1];
        ch[y][sit] = u, fa[u] = y;
        ch[x][sit ^ 1] = y, fa[y] = x;
        fa[x] = z;
        pull(y), pull(x);
    }

    void splay(int x){
        for (int f = fa[x]; fa[x]; f = fa[x]){
            if (fa[f]) rotate(get(f) == get(x) ? f : x);
            rotate(x);
        }
        root = x;
    }

    void insert(int x){
        int to_ins;
        if (!root){
            root = to_ins = newnode();
        }
        else{
            int p = root;
            while (true){
                if (key[p] == x){
                    to_ins = p;
                    break;
                }
                int &next = ch[p][x > key[p]];
                if (next == 0){
                    next = to_ins = newnode();
                    fa[to_ins] = p;
                    break;
                }
                p = next;
            }
        }
        key[to_ins] = x;
        ++ cnt[to_ins];
        pull(to_ins);
        splay(to_ins);
    }

    int find(int x){
        int p = root;
        while (p){
            if (key[p] == x){
                splay(p);
                return p;
            }
            p = ch[p][x > key[p]];
        }
        return -1;
    }

    int pre(){
        int p = ls(root);
        if (!p) return p;
        while (rs(p)) p = rs(p);
        splay(p);
        return p;
    }

    int nxt(){
        int p = rs(root);
        if (!p) return p;
        while (ls(p)) p = ls(p);
        splay(p);
        return p;
    }

    int prev(int x){
        insert(x);
        int result = pre();
        result = result == 0 ? INT_MIN : key[result];
        erase(x);
        return result;
    }

    int next(int x){
        insert(x);
        int result = nxt();
        result = result == 0 ? INT_MAX : key[result];
        erase(x);
        return result;
    }

    void erase(int x){
        find(x);
        if (cnt[root] > 1){
            -- cnt[root];
            pull(root);
            return;
        }
        if (!ls(root) && !rs(root)){
            root = 0;
            return;
        }
        if (!ls(root)){
            fa[rs(root)] = 0;
            root = rs(root);
            return;
        }
        if (!rs(root)){
            fa[ls(root)] = 0;
            root = ls(root);
            return;
        }
        int rt = root, p = pre();
        fa[rs(rt)] = p;
        rs(p) = rs(rt);
        pull(root);
    }

    int rk(int x){
        int p = root, ret = 0;
        while (p){
            if (key[p] == x){
                ret += sz[ls(p)];
                splay(p);
                return ret;
            }
            if (x > key[p]) ret += sz[ls(p)] + cnt[p];
            p = ch[p][x > key[p]];
        }
        return ret;
    }

    int query(int k){
        int p = root;
        while (p){
            if (k <= sz[ls(p)]) p = ls(p);
            else if (k <= sz[ls(p)] + cnt[p]) return key[p];
            else k -= sz[ls(p)] + cnt[p], p = rs(p);
        }
        return -1;
    }
};

Splay splay;

int main(){
    splay.init();
    int n;
    scanf("%d", &n);
    while (n --){
        int op, x;
        scanf("%d%d", &op, &x);
        switch (op) {
            case 1: splay.insert(x); break;
            case 2: splay.erase(x); break;
            case 3: printf("%d\n", splay.rk(x) + 1); break;
            case 4: printf("%d\n", splay.query(x)); break;
            case 5: printf("%d\n", splay.prev(x)); break;
            case 6: printf("%d\n", splay.next(x)); break;
        }
    }
    return 0;
}
