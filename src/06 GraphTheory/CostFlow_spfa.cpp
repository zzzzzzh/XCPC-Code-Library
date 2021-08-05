//Min cost feasible flow or max flow
//When modifying to long long: inf, dis[], cost, memset(dis, inf), edge.w, addedge(w)
namespace MCMF {
    const int max_N = 2333; //vertexes number
    const int inf = 0x3f3f3f3f; //a number larger than all dis

    int M, dis[max_N], vis[max_N], pre[max_N], flow, cost;

    struct edge {
        int v, c, w; //vertex, capacity, weight
    };

    std::vector<edge> e; //all edges
    std::vector<int> vec[max_N]; //edge indexes for each vertex

    bool spfa(int n, int s, int t) {
        static int Q[max_N];
        int head = 0, tail = 0;
        memset(dis + 1, 0x3f, n * sizeof(int));
        memset(vis + 1, 0, n * sizeof(int));
        dis[s] = 0, vis[s] = 1, Q[tail++] = s;
        while (head != tail) {
            int u = Q[head++];
            vis[u] = 0;
            if (head == max_N) head = 0;
            for (auto it : vec[u]) {
                if (!e[it].c) continue;
                int v = e[it].v;
                auto tmp = dis[u] + e[it].w;
                if (tmp < dis[v]) {
                    pre[v] = it, dis[v] = tmp;
                    if (!vis[v]) {
                        vis[v] = 1, Q[tail++] = v;
                        if (tail == max_N) tail = 0;
                    }
                }
            }
        }
        return dis[t] < inf;
    }

    /**
     * Add an edge from u to v
     * @param u from
     * @param v to
     * @param c capacity
     * @param w weight
     */
    void addEdge(int u, int v, int c, int w) {
        e.push_back({v, c, w});
        vec[u].push_back(M++);
        e.push_back({u, 0, -w});
        vec[v].push_back(M++);
    }

    /**
     * Computing min cost max flow or feasible flow from s to t.
     * The results are stored in variables flow and cost
     * @param n vertexes number
     * @param s from
     * @param t to
     */
    void minCost(int n, int s, int t) {
        while (spfa(n, s, t) && dis[t] < 0) { //for feasible flow
        // while (spfa(n, s, t)) { //for max flow
            int adt = ~0U >> 1;
            for (int x = t; x != s; x = e[pre[x] ^ 1].v) {
                adt = std::min(adt, e[pre[x]].c);
            }
            flow += adt;
            cost += adt * dis[t];
            for (int x = t; x != s; x = e[pre[x] ^ 1].v) {
                e[pre[x]].c -= adt;
                e[pre[x] ^ 1].c += adt;
            }
        }
    }
    
    /**
     * clear
     * @param n vertexes number
     */
    void clear(int n) {
        e.clear();
        for (int i = 1; i <= n; ++i) {
            std::vector<int>().swap(vec[i]);
        }
        M = flow = cost = 0;
    }
};
