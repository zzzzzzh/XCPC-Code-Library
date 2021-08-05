#include <bits/stdc++.h>

namespace GM{
    const int N = 1010;

    std::vector <int> e[N];
    int color[N], queue[N];
    int nxt[N], pre[N], fa[N], vis[N];
    int head, tail, viscnt;

    int find(int u){
        return u == fa[u] ? u : (fa[u] = find(fa[u]));
    }

    void addedge(int u, int v){
        e[u].emplace_back(v);
        e[v].emplace_back(u);
    }

    int lca(int u, int v){
        ++ viscnt;
        u = find(u), v = find(v);
        while (true){
            if (u){
                if (vis[u] == viscnt) return u;
                vis[u] = viscnt;
                u = find(pre[nxt[u]]);
            }
            std::swap(u, v);
        }
    }

    void blossom(int u, int v, int l){
        while (find(u) != l){
            pre[u] = v;
            if (color[nxt[u]] == 1){
                color[nxt[u]] = 0;
                queue[tail ++] = nxt[u];
            }
            fa[u] = fa[nxt[u]] = l;
            v = nxt[u], u = pre[v];
        }
    }

    void match(int start){
        memset(pre, 0, sizeof(pre));
        memset(color, -1, sizeof(color));
        head = 0, tail = 0;
        queue[tail ++] = start;
        color[start] = 0;
        for ( ; head < tail; ++ head){
            int u = queue[head];
            for (auto v : e[u]){
                if (color[v] == -1){
                    color[v] = 1, pre[v] = u;
                    if (nxt[v] == 0){ // found augment path
                        int x = u, y = v;
                        while (x){
                            int tmp = nxt[x];
                            nxt[x] = y, nxt[y] = x;
                            x = pre[tmp], y = tmp;
                        }
                        assert(!x && !y);
                        return;
                    }
                    queue[tail ++] = nxt[v];
                    color[nxt[v]] = 0;
                }
                else if (color[v] == 0 && find(u) != find(v)){
                    int l = lca(u, v);
                    blossom(u, v, l), blossom(v, u, l);
                }
            }
        }
    }

    int solve(int n){
        memset(vis, 0, sizeof(vis));
        memset(nxt, 0, sizeof(nxt));
        viscnt = 0;
        for (int i = 1; i <= n; ++ i){
            if (nxt[i]){
                continue;
            }
            for (int j = 1; j <= n; ++ j){
                fa[j] = j;
            }
            match(i);
        }
        int ans = 0;
        for (int i = 1; i <= n; ++ i){
            ans += nxt[i] != 0;
        }
        return ans / 2;
    }
}

int main(){
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 0; i <= n; ++ i){
        GM::e[i].clear();
    }
    for (int i = 0, u, v; i < m; ++ i){
        scanf("%d%d", &u, &v);
        GM::addedge(u, v);
    }
    int ans = GM::solve(n);
    printf("%d\n", ans);
    for (int i = 1; i <= n; ++ i){
        printf("%d%c\n", GM::nxt[i], " \n"[i == n]);
    }
    return 0;
}