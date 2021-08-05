<TeX>带权带花树，时间复杂度 $\mathcal{O}(n^{3})$。这份代码写得超烂，UOJ上排倒数第三，建议大家别用...</TeX>
#include <bits/stdc++.h>

namespace GMW{
    const int N = 1010;
    const int M = 100100;
    using T = long long;
    const T INF = 1e18;
    const bool max_cardinality = false;

    int n;
    std::vector <int> e[N];
    int color[N * 2], slack[N * 2], slack1[N * 2][N * 2]; T dual[N * 2];
    int queue[N], head, tail; bool inque[N * 2];
    int vis[N], viscnt;
    int nxt[N], pre[N]; // nxt is the matching edge, pre is the parent edge in the alternating forest
    int uu[M], vv[M], ecnt; T w[M];

    std::vector <int> children[N * 2], children_e[N * 2], blossom_queue;
    int fa[N * 2], top[N * 2], base[N * 2];

    inline int peer(int u, int edge){ return top[uu[edge]] == top[u] ? vv[edge] : uu[edge]; }
    inline T get_slack(int edge){ return edge == -1 ? LLONG_MAX : dual[uu[edge]] + dual[vv[edge]] - 2 * w[edge]; }
    inline void update(int &e1, int e2){ if (e1 == -1 || get_slack(e1) > get_slack(e2)){e1 = e2;} }

    int new_blossom(){
        int id = blossom_queue.back();
        blossom_queue.pop_back();
        children[id].clear();
        children_e[id].clear();
        fa[id] = dual[id] = 0;
        slack[id] = -1;
        memset(slack1[id], -1, sizeof(slack1[id]));
        return id;
    }

    void addedge(int u, int v, T weight){
        uu[ecnt] = u;
        vv[ecnt] = v;
        w[ecnt] = weight;
        e[u].emplace_back(ecnt);
        e[v].emplace_back(ecnt);
        ecnt ++;
    }

    void settop(int u, int tp){
        top[u] = tp;
        for (auto v : children[u]){
            settop(v, tp);
        }
    }

    inline void push(int u){
        if (inque[u]) return;
        inque[u] = true;
        queue[tail ++] = u;
        for (auto edge : e[u]){
            int v = uu[edge] ^ vv[edge] ^ u;
            if (color[top[v]] != 0){
                update(slack[v], edge);
            }
        }
    }

    void slack_type3(int b){
        slack[b] = -1;
        if (b <= n){
            for (auto edge : e[b]){
                int v = b ^ uu[edge] ^ vv[edge];
                if (top[b] != top[v] && color[top[v]] == 0){
                    update(slack1[b][top[v]], edge);
                    update(slack[b], edge);
                }
            }
            slack1[b][0] = 0;
            return;
        }
        for (auto u : children[b]){
            if (slack1[u][0] == -1){
                slack_type3(u);
            }
            for (int i = 1; i <= 2 * n; ++ i){
                if (top[i] != top[b] && color[top[i]] == 0){
                    update(slack1[b][top[i]], slack1[u][i]);
                    update(slack[b], slack1[u][i]);
                }
            }
        }
        slack1[b][0] = 0;
    }

    void push_to_queue(int u){
        if (u <= n) push(u);
        for (auto v : children[u]){
            push_to_queue(v);
        }
    }

    void setcolor(int u, int col){
        u = top[u];
        color[u] = col;
        if (col == 0){
            push_to_queue(u);
            slack_type3(u);
        }
        else if (col == 1){
            int edge = nxt[base[u]];
            assert(edge >= 0);
            int v = top[peer(u, edge)];
            setcolor(v, 0);
        }
    }

    int lca(int u, int v){ // to modify
        ++ viscnt;
        u = top[u], v = top[v];
        while (u || v){
            if (u){
                if (vis[u] == viscnt) return u;
                vis[u] = viscnt;
                u = top[peer(u, nxt[base[u]])];
                u = top[peer(u, pre[u])];
            }
            std::swap(u, v);
        }
        return 0;
    }

    int adjust(int b, int edge){
        auto get_child = [&](int u){
            while (u && fa[u] != b){
                u = fa[u];
            }
            return u;
        };
        int ch = get_child(uu[edge]) | get_child(vv[edge]);
        int pos = std::find(children[b].begin(), children[b].end(), ch) - children[b].begin();
        if (pos % 2){
            std::reverse(children[b].begin() + 1, children[b].end());
            std::reverse(children_e[b].begin(), children_e[b].end());
            int sz = children[b].size();
            pos = (sz - pos) % sz;
        }
        return pos;
    }

    void augment_blossom(int b, int edge){
        if (b <= n) return;
        int pos = adjust(b, edge);
        auto &ch = children[b];
        auto &che = children_e[b];
        augment_blossom(ch[pos], edge);
        for (int i = pos - 1; i >= 0; i -= 2){
            int ed = che[i - 1];
            augment_blossom(ch[i - 1], ed);
            augment_blossom(ch[i], ed);
            nxt[uu[ed]] = nxt[vv[ed]] = ed;
        }
        std::rotate(ch.begin(), ch.begin() + pos, ch.end());
        std::rotate(che.begin(), che.begin() + pos, che.end());
        base[b] = base[ch[0]];
    }

    bool linkSS(int u, int v, int edge){ // return found augment path
        int l = lca(u, v);
        if (l == 0){ // found an augment path
            for (int _ = 0; _ < 2; ++ _){
                int x = u, y = v, e1 = edge;
                while (x){
                    int tmp = nxt[base[top[x]]];
                    nxt[x] = e1;
                    augment_blossom(top[x], e1);
                    if (tmp == -1) break;
                    y = peer(x, tmp);
                    e1 = pre[top[y]];
                    x = peer(y, e1), y = peer(x, e1);
                    nxt[y] = e1;
                    augment_blossom(top[y], e1);
                }
                std::swap(u, v);
            }
            return true;
        }
        int id = new_blossom();
        auto &ch = children[id];
        auto &che = children_e[id];
        for (int _ = 0; _ < 2; ++ _){
            int x = top[u], e1 = edge;
            while (true){
                ch.emplace_back(x);
                che.emplace_back(e1);
                if (x == l) break;
                int e2 = nxt[base[x]];
                int y = top[peer(x, e2)];
                ch.emplace_back(y);
                che.emplace_back(e2);
                e1 = pre[y];
                x = top[peer(y, e1)];
            }
            std::swap(u, v);
            std::reverse(ch.begin(), ch.end());
            std::reverse(che.begin(), che.end());
            if (_ == 0) che.pop_back();
        }
        ch.pop_back();
        base[id] = base[ch[0]];
        pre[id] = pre[ch[0]];
        for (auto x : ch){
            fa[x] = id;
        }
        settop(id, id);
        setcolor(id, 0);
        return false;
    }

    void extract(int u){
        int edge = pre[u];
        int pos = adjust(u, edge);
        auto &ch = children[u];
        auto &che = children_e[u];
        for (int i = 0; i < int(ch.size()); ++ i){
            fa[ch[i]] = 0, settop(ch[i], ch[i]);
            setcolor(ch[i], i <= pos ? ((i % 2) ^ 1) : -1);
        }
        pre[ch[pos]] = edge;
        for (int i = 0; i < pos; i += 2){
            pre[ch[i]] = che[i];
        }
        blossom_queue.emplace_back(u);
        top[u] = 0;
    }

    bool match(){
        memset(color, -1, sizeof(color));
        memset(slack, -1, sizeof(slack));
        memset(slack1, -1, sizeof(slack1));
        memset(pre, -1, sizeof(pre));
        memset(inque, 0, sizeof(inque));
        head = tail = 0;
        for (int i = 1; i <= n; ++ i){
            if (nxt[i] == -1){
                setcolor(i, 0);
            }
        }
        while (true){
            for ( ; head < tail; ++ head){
                int u = queue[head];
                for (auto edge : e[u]){
                    if (get_slack(edge)){
                        continue;
                    }
                    int v = peer(u, edge);
                    if (top[u] == top[v]){
                        continue;
                    }
                    if (color[top[v]] == -1){ // must be a matched point
                        assert(nxt[v] != -1);
                        pre[top[v]] = edge;
                        setcolor(v, 1);
                    }
                    else if (color[top[v]] == 0){
                        if (linkSS(u, v, edge)){
                            return true;
                        }
                    }
                }
            }
            int type = 0, pos = 0;
            T delta = LLONG_MAX;
            for (int i = 1; i <= n; ++ i){
                T val = get_slack(slack[i]);
                if (color[top[i]] == -1 && val < delta){
                    delta = val, type = 1, pos = i;
                }
            }
            for (int i = 1; i <= 2 * n; ++ i){
                T val = get_slack(slack[i]) / 2;
                if (top[i] == i && color[i] == 0 && val < delta){
                    delta = val, type = 2, pos = i;
                }
            }
            for (int i = n + 1; i <= 2 * n; ++ i){
                if (top[i] == i && color[i] == 1 && dual[i] / 2 < delta){
                    delta = dual[i] / 2, type = 3, pos = i;
                }
            }
            if (delta >= INF){
                break;
            }
            if (!max_cardinality){
                T min_value = *std::min_element(dual + 1, dual + 1 + n);
                if (min_value < delta){
                    break;
                }
            }
            for (int i = 1; i <= n; ++ i){
                int col = color[top[i]];
                dual[i] += col == 0 ? -delta : col == 1 ? delta : 0;
            }
            for (int i = n + 1; i <= 2 * n; ++ i){
                if (top[i] == i){
                    int col = color[i];
                    dual[i] += col == 0 ? 2 * delta : col == 1 ? -2 * delta : 0;
                }
            }
            if (type == 1){
                assert(nxt[pos] != -1);
                pre[top[pos]] = slack[pos];
                setcolor(pos, 1);
            }
            else if (type == 2){
                int edge = slack[pos];
                if (linkSS(uu[edge], vv[edge], edge)){
                    return true;
                }
            }
            else{ // type == 3
                extract(pos);
            }
        }
        return false;
    }

    // max weight (max cardinality, can control via max_cardinality) general matching
    // remember to set n
    T solve(){
        T max_weight = *std::max_element(w, w + ecnt);
        for (int i = 1; i <= n; ++ i){
            dual[i] = max_weight;
            blossom_queue.emplace_back(i + n);
            fa[i] = 0;
            top[i] = base[i] = i;
        }
        while (match())
            ;
        T ans = 0;
        for (int i = 1; i <= n; ++ i){
            if (nxt[i] != -1){
                ans += w[nxt[i]];
            }
        }
        return ans / 2;
    }

    void init(){
        for (int i = 0; i < N; ++ i){
            e[i].clear();
        }
        ecnt = 0;
        viscnt = 0;
        memset(vis, 0, sizeof(vis));
        memset(nxt, -1, sizeof(nxt));
        memset(top, 0, sizeof(top));
        blossom_queue.clear();
    }
}

const int N = 1010;
using ll = long long;

ll w[N][N];

std::mt19937_64 rnd(std::chrono::steady_clock::now().time_since_epoch().count());

ll rand(ll n){
    return rnd() % n;
}

void testcase(){
    int n = 100;
    int e = 1000000;
    for (int i = 0; i < e; ++ i){
        int x = rand(n) + 1, y = rand(n) + 1;
        ll weight = rand(100000000000LL) + 1;
        w[x][y] = w[y][x] = weight;
    }
    int cnt = 0;
    for (int i = 1; i <= n; ++ i){
        for (int j = 1; j < i; ++ j){
            cnt += w[i][j] > 0;
        }
    }
    GMW::init();
    GMW::n = n;
    for (int i = 1; i <= n; ++ i){
        for (int j = 1; j < i; ++ j){
            if (w[i][j] > 0){
                GMW::addedge(i, j, w[i][j]);
            }
        }
    }
}

int main(){
    for (int _ = 0; _ < 2; ++ _){ // test init
        testcase();
    }
    GMW::init();
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 0, u, v, w; i < m; ++ i){
        scanf("%d%d%d", &u, &v, &w);
        GMW::addedge(u, v, w);
    }
    GMW::n = n;
    printf("%lld\n", GMW::solve());
    for (int i = 1; i <= n; ++ i){
        int ans = 0;
        if (GMW::nxt[i] != -1){
            int edge = GMW::nxt[i];
            ans = i ^ GMW::uu[edge] ^ GMW::vv[edge];
        }
        printf("%d%c", ans, " \n"[i == n]);
    }
    return 0;
}
