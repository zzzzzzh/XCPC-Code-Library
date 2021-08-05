#include <bits/stdc++.h>

const int N = 600010;

std::vector<int> e[N], e1[N]; // points in e1 is of the same order of the ring
std::vector<int> stack;
int dfn[N], low[N], dfncnt;
int ncnt; // set ncnt = n before run tarjan

void addedge(int u, int v){
    e1[u].emplace_back(v);
    e1[v].emplace_back(u);
}

void tarjan(int u, int fa){
    dfn[u] = low[u] = ++ dfncnt;
    stack.emplace_back(u);
    for (auto v : e[u]){
        if (v == fa){
            continue;
        }
        if (dfn[v]){
            low[u] = std::min(low[u], dfn[v]);
        }
        else{
            tarjan(v, u);
            low[u] = std::min(low[u], low[v]);
            if (dfn[u] == low[v]){
                ++ ncnt;
                addedge(u, ncnt);
                while (true){
                    int x = stack.back();
                    stack.pop_back();
                    addedge(x, ncnt);
                    if (x == v) break;
                }
            }
            else if (dfn[u] < low[v]){
                addedge(u, v);
                stack.pop_back();
            }
        }
    }
}
