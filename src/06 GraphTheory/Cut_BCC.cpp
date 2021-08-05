// 割点可能存在于多个点双中, 建图时对每个点双新建点, 
// 向除深度最小的点之外的所有点连边, 再从深度最小的点向其连边
// 这样原图中每个点的信息在树中对应的节点和父亲节点处维护即可
// 如果u到v的lca是新建点，则lca的父亲节点的信息也应该考虑
namespace BCC {
    const int max_N = (int) 2e5 + 21; //点开两倍
    int st[max_N], top;
    int dfn[max_N], low[max_N], cnt;
    int par[max_N], tot;
    std::vector<int> vec[max_N], bcc[max_N];
    bool cut[max_N];
    void tarjan(int u, int p) {
        st[++top] = u;
        dfn[u] = low[u] = ++cnt;
        int ch = 0;
        for (auto v : vec[u]) {
            if (v == p) continue;
            ++ch;
            if (!dfn[v]) {
                tarjan(v, u);
                low[u] = std::min(low[u], low[v]);
                if (dfn[u] <= low[v]) {
                    cut[u] = true;
                    ++tot;
                    do {
                        bcc[tot].push_back(st[top]);
                        assert(!par[st[top]]);
                        par[st[top]] = tot;
                    } while (st[top--] != v);
                    bcc[u].push_back(tot);
                    par[tot] = u;
                }
            } else if (dfn[v] < dfn[u]) {
                low[u] = std::min(low[u], dfn[v]);
            }
        }
        if (!p && ch == 1) {
            cut[u] = false;
        }
    }
};

// 有时候需要找出点双中的所有边, 可以使用下面的写法, 增加维护边的栈
// 下面为求所有恰好只出现在一个简单环中的所有边的代码
/*
namespace BCC {
    const int max_N = (int) 1e5 + 21;
    const int max_M = max_N * 21;

    int n, m, dfn[max_N], low[max_N], cnt, Pst[max_N], Ptop, Est[max_M], Etop, ans;

    std::vector<std::pair<int, int>> vec[max_N]; // first : v, second : id

    bool ret[max_M];

    void tarjan(int u, int pre) {
        dfn[u] = low[u] = ++cnt;
        Pst[++Ptop] = u;
        for (auto e : vec[u]) {
            int v = e.first, w = e.second;
            if (w == pre) continue;
            if (!dfn[v]) {
                Est[++Etop] = w;
                tarjan(v, w);
                low[u] = std::min(low[u], low[v]);
                if (dfn[u] <= low[v]) {
                    int Psiz = 1, Esiz = 0;
                    do {
                        ++Psiz;
                    } while (Pst[Ptop--] != v);
                    std::vector<int> tmp;
                    do {
                        ++Esiz;
                        tmp.push_back(Est[Etop]);
                    } while (Est[Etop--] != w);
                    if (Psiz == Esiz) {
                        for (auto i : tmp) {
                            ++ans;
                            ret[i] = true;
                        }
                    }
                }
            } else if (dfn[v] < dfn[u]) {
                Est[++Etop] = w;
                low[u] = std::min(low[u], dfn[v]);
            }
        }
    }
};
*/