<TeX>
这是带权拟阵交的模板，`value` 数组的值设为对应 `Object` 的点权。
如果没有理解错的话，所求是达到秩最大前提下的的最大权拟阵交。如果单纯要求最大权拟阵交，注意到每次只扩展一个点，且求出来的都是当前大小下的最大权，所以你懂的。
理论分析复杂度是 $\mathcal{O}(r^{3}n^{2})$，因为涉及到 spfa。其中 $r=\max(r_{1},r_{2})$，即两拟阵基的大小的最大值。
但是实际上可能卡不满，那么复杂度会和不带权的拟阵交一样。
</TeX>
#include <bits/stdc++.h>

struct Object{
};

std::vector <Object> objects;

// given an independent set `set`, figure out all other indexes in `to_check`
// which can be add to `set`, and remain independent (matroid1)
std::vector <int> check1(const std::vector <int> &set, const std::vector <int> &to_check);

// given an independent set `set`, figure out all other indexes in `to_check`
// which can be add to `set`, and remain independent (matroid2)
std::vector <int> check2(const std::vector <int> &set, const std::vector <int> &to_check);

const int N = 100010;
using pii = std::pair <int, int>;

std::vector <int> e[N];
int value[N];
pii dis[N];

std::vector <int> matroid_intersection(){
    int n = objects.size();
    std::vector <int> set;
    while (true){
        for (int i = 0; i < n; ++ i){
            e[i].clear();
            dis[i] = {INT_MIN, INT_MIN};
        }
        std::vector <int> to_check;
        std::vector <bool> vis(n);
        for (auto u : set){
            vis[u] = true;
        }
        for (int i = 0; i < n; ++ i){
            if (!vis[i]){
                to_check.emplace_back(i);
            }
        }
        std::vector <int> x1 = check1(set, to_check);
        std::vector <int> x2 = check2(set, to_check);
        int sz = set.size();
        for (int i = 0; i < sz; ++ i){
            std::vector <int> nset;
            for (int j = 0; j < sz; ++ j){
                if (i != j){
                    nset.emplace_back(set[j]);
                }
            }
            std::vector <int> edge = check1(nset, to_check);
            for (auto u : edge){
                e[set[i]].emplace_back(u);
            }
            edge = check2(nset, to_check);
            for (auto u : edge){
                e[u].emplace_back(set[i]);
            }
        }
        std::vector <int> pre(n, -1);
        std::queue <int> queue;
        std::vector <bool> inque(n);
        for (auto u : x1){
            queue.emplace(u);
            inque[u] = true;
            dis[u] = {value[u], 0};
        }
        int found = -1;
        while (!queue.empty()){
            int u = queue.front();
            queue.pop();
            inque[u] = false;
            for (auto v : e[u]){
                auto [weight, edge] = dis[u];
                weight += vis[v] ? -value[v] : value[v];
                -- edge;
                pii ndis = {weight, edge};
                if (ndis > dis[v]){
                    dis[v] = ndis;
                    if (!inque[v]){
                        queue.emplace(v);
                        inque[v] = true;
                    }
                    pre[v] = u;
                }
            }
        }
        pii max = {INT_MIN, INT_MIN};
        for (auto u : x2){
            if (dis[u] > max){
                max = dis[u];
                found = u;
            }
        }
        if (found == -1){
            break;
        }
        while (found != -1){
            vis[found] = !vis[found];
            found = pre[found];
        }
        set.clear();
        for (int i = 0; i < n; ++ i){
            if (vis[i]){
                set.emplace_back(i);
            }
        }
    }
    return set;
}

int main(){
    matroid_intersection();
}
