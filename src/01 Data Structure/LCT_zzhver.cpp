#include <bits/stdc++.h>

const int N = 100010;

struct LCT{ // 0 必须时刻为 0，请在必要的时候 clear(0)
#define ls(u) (ch[(u)][0])
#define rs(u) (ch[(u)][1])

    int ch[N][2], fa[N];
    int sz[N], sz1[N], sz2[N];
    int lazy[N];

    void clear(int x){
        ls(x) = rs(x) = 0;
        sz2[x] = 0;
        fa[x] = lazy[x] = 0;
    }

    void init(int n){
        for (int i = 0; i <= n; ++ i){
            clear(i);
        }
    }

    void pull(int u){
        sz[u] = sz[ls(u)] + sz[rs(u)] + 1;
        sz1[u] = sz1[ls(u)] + sz1[rs(u)] + 1 + sz2[u];
    }

    void push(int u){
        if (lazy[u]){
            std::swap(ls(u), rs(u));
            lazy[ls(u)] ^= 1, lazy[rs(u)] ^= 1;
            lazy[u] = 0;
        }
    }

    int get(int x){
        return x == rs(fa[x]);
    }

    bool isroot(int x){
        return ls(fa[x]) != x && rs(fa[x]) != x;
    }

    void update(int x){
        if (!isroot(x)) update(fa[x]);
        push(x);
    }

    void rotate(int x){
        int y = fa[x], z = fa[y], sit = get(x);
        if (!isroot(y)) ch[z][get(y)] = x;
        int u = ch[x][sit ^ 1];
        ch[y][sit] = u, fa[u] = y;
        ch[x][sit ^ 1] = y, fa[y] = x;
        fa[x] = z;
        pull(y), pull(x);
    }

    void splay(int x){
        update(x);
        for (int f = fa[x]; !isroot(x); f = fa[x]){
            if (!isroot(f)) rotate(get(f) == get(x) ? f : x);
            rotate(x);
        }
    }

    int access(int x){
        int p = 0;
        for ( ; x; p = x, x = fa[x]){
            splay(x);
            sz2[x] += sz1[rs(x)] - sz1[p];
            rs(x) = p, pull(x);
        }
        return p;
    }

    void makeroot(int x){
        x = access(x);
        // 注意，当维护的信息与顺序相关时，这里就出问题了，
        // 通常的做法是同时维护正序和逆序的信息
        lazy[x] ^= 1;
    }

    int find(int x){
        access(x), splay(x);
        while (push(x), ls(x)){
            x = ls(x);
        }
        splay(x);
        return x;
    }

    bool link(int u, int v){
        makeroot(u);
        if (find(v) == u) return false;
        makeroot(v); // 维护 sz1, sz2 时需要加上，否则 sz2 的更改更新不上去
        splay(u);
        fa[u] = v;
        sz2[v] += sz1[u];
        return true;
    }

    void split(int u, int v){ // set u to root, set v to root of aux tree
        makeroot(u), access(v), splay(v);
    }

    void cut(int u, int v){ // call after checking (u, v) exists!
        split(u, v);
        fa[u] = ls(v) = 0;
        pull(v);
    }
};

LCT lct;
char s[N];

int main(){
    int n, m;
    scanf("%d%d", &n, &m);
    lct.init(n);
    return 0;
}
