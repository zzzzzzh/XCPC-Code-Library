#include <bits/stdc++.h>

using ll = long long;

struct Dinic{
    const static int N = 1010;
    const static ll INF = 0x3f3f3f3f3f3f3f3f;

    struct Edge{
        int to, n;
        ll c;
    };

    std::vector <Edge> e;
    int ecnt, cur[N], head[N], dis[N];
    int n, s, t;

    bool bfs(){
        memset(dis + 1, -1, sizeof(dis[0]) * n);
        dis[s] = 0;
        std::queue<int> queue;
        queue.emplace(s);
        while (!queue.empty()){
            int u = queue.front();
            queue.pop();
            for (int i = head[u]; ~i; i = e[i].n){
                if (!e[i].c) continue;
                int v = e[i].to;
                if (dis[v] == -1){
                    dis[v] = dis[u] + 1;
                    queue.emplace(v);
                }
            }
        }
        return dis[t] != -1;
    }

    ll dfs(int u, ll a){
        if (u == t) return a;
        ll ret = 0;
        for (int &i = cur[u]; ~i; i = e[i].n){
            int v = e[i].to;
            if (!e[i].c || dis[u] + 1 != dis[v]) continue;
            ll tmp = dfs(v, std::min(a - ret, e[i].c));
            if (tmp){
                e[i].c -= tmp, e[i ^ 1].c += tmp;
                if ((ret += tmp) == a) break;
            }
        }
        if (!ret) dis[u] = -1;
        return ret;
    }

    void init(int n, int s, int t){
        this -> n = n, this -> s = s, this -> t = t;
        memset(head + 1, -1, sizeof(head[0]) * n);
        e.clear(), ecnt = 0;
    }

    void addedge(int u, int v, ll c){
        e.push_back({v, head[u], c});
        head[u] = ecnt ++;
        e.push_back({u, head[v], 0});
        head[v] = ecnt ++;
    }

    ll maxflow(){
        ll ret = 0;
        while (bfs()){
            memcpy(cur + 1, head + 1, sizeof(head[0]) * n);
            ret += dfs(s, INF);
        }
        return ret;
    }
};
