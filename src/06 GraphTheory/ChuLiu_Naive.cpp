<TeX>最小树形图的朱刘算法，复杂度 $\mathcal{O}(nm)$，不支持输出方案</TeX>
namespace ChuLiu {
    const int max_N = (int) 1e3 + 21, max_M = (int) 1e4 + 21, inf = 0x7f7f7f7f;
    
    struct edge { int u, v, w; } e[max_M];
    
    int n, m, rt, pre[max_N], id[max_N], cnt, in[max_N], vis[max_N];

    inline void init(int N) { 
        n = N, m = 0;
    }

    inline void add_edge(int u, int v, int w) { 
        e[++m] = {u, v, w};
    }

    inline int calc() {
        int ans = 0, i, j, k, tmp;
        while (true) {
            for (i = 1; i <= n; ++i) in[i] = inf;
            for (i = 1; i <= m; ++i)
                if (e[i].u != e[i].v && e[i].w < in[e[i].v]) {
                    in[e[i].v] = e[i].w, pre[e[i].v] = e[i].u;
                }
            in[rt] = 0;
            for (i = 1; i <= n; ++i)
                if (in[i] == inf) return -1; //无解时返回-1
            cnt = 0;
            for (i = 1; i <= n; ++i) vis[i] = id[i] = 0;
            for (i = 1; i <= n; ++i) {
                for (j = i; j != rt && vis[j] != i && !id[j]; j = pre[j]) vis[j] = i;
                if (j == rt || id[j]) continue;
                id[j] = ++cnt;
                for (k = pre[j]; k != j; k = pre[k]) id[k] = cnt;
            }
            if (!cnt)break;
            for (i = 1; i <= n; ++i)
                if (!id[i]) id[i] = ++cnt;
            for (i = 1; i <= m; ++i) {
                tmp = in[e[i].v];
                e[i].u = id[e[i].u], e[i].v = id[e[i].v];
                if (e[i].u != e[i].v) e[i].w -= tmp;
            }
            n = cnt;
            rt = id[rt];
        }
        return ans;
    }
};
