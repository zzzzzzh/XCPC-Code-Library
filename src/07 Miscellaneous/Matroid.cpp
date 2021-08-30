<TeX>
一般拟阵题都不是单纯抄板就可以通过的，甚至check1和check2本身需要融合进代码进行优化等等。所以只好给出一个大致框架。
暴力做的复杂度是 $\mathcal{O}(r^{2}n)$，其中 $r=\max(r_{1},r_{2})$，即两拟阵基的大小的最大值。
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

std::vector <int> e[N];

std::vector <int> matroid_intersection(){
    int n = objects.size();
    std::vector <int> set;
    while (true){
        for (int i = 0; i < n; ++ i){
            e[i].clear();
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
        std::vector <bool> vis1(n), vis2(n);
        std::vector <int> pre(n, -1);
        std::queue <int> queue;
        for (auto u : x1){
            vis1[u] = true;
            queue.emplace(u);
        }
        for (auto u : x2){
            vis2[u] = true;
        }
        int found = -1;
        while (!queue.empty()){
            int u = queue.front();
            queue.pop();
            if (vis2[u]){
                found = u;
                break;
            }
            for (auto v : e[u]){
                if (vis1[v]){
                    continue;
                }
                vis1[v] = true;
                pre[v] = u;
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
