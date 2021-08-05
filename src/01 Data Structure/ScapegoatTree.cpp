#include <bits/stdc++.h>

const int N = 100010;

struct ScapegoatTree{
#define ls(u) (ch[(u)][0])
#define rs(u) (ch[(u)][1])

    constexpr static double alpha = 0.75;
    int node_sz;
    int root;
    int ch[N][2];
    int key[N];
    int sz[N], sz_zero[N], cnt[N], tot[N];
    int use_nodes[N], use_node_cnt;

    int newnode(){
        int ret = ++ node_sz;
        ls(ret) = rs(ret) = 0;
        sz[ret] = sz_zero[ret] = cnt[ret] = tot[ret] = 0;
        return ret;
    }

    void init(){
        node_sz = root = 0;
    }

    bool check(int u){
        double bound = alpha * sz[u];
        return sz_zero[u] >= bound || sz[ls(u)] >= bound || sz[rs(u)] >= bound;
    }

    void pull(int u){
        sz[u] = sz[ls(u)] + sz[rs(u)] + 1;
        sz_zero[u] = sz_zero[ls(u)] + sz_zero[rs(u)] + !cnt[u];
        tot[u] = tot[ls(u)] + tot[rs(u)] + cnt[u];
    }

    void search(int u){
        if (!u) return;
        search(ch[u][0]);
        if (cnt[u]){
            use_nodes[use_node_cnt ++] = u;
        }
        search(ch[u][1]);
    }

    int rebuild(int l, int r){
        if (l > r) return 0;
        int mid = (l + r) / 2;
        int rt = use_nodes[mid];
        ls(rt) = rebuild(l, mid - 1);
        rs(rt) = rebuild(mid + 1, r);
        pull(rt);
        return rt;
    }

    void rebuild(int &u){
        use_node_cnt = 0;
        search(u);
        u = rebuild(0, use_node_cnt - 1);
    }

    void insert(int &u, int value){
        if (!u){
            u = newnode();
            key[u] = value;
            cnt[u] = 1;
        }
        else{
            if (value == key[u]) ++ cnt[u];
            else if (value < key[u]) insert(ls(u), value);
            else insert(rs(u), value);
        }
        pull(u);
        if (check(u)) rebuild(u);
    }

    void del(int &u, int value){
        if (!u){
            return;
        }
        else{
            if (value == key[u]) -- cnt[u];
            else if (value < key[u]) del(ls(u), value);
            else del(rs(u), value);
        }
        pull(u);
        if (check(u)) rebuild(u);
    }

    int upperbound(int u, int value){
        if (!u || !tot[u]){
            return INT_MAX;
        }
        if (value >= key[u]){
            return upperbound(rs(u), value);
        }
        int ret = upperbound(ls(u), value);
        if (ret != INT_MAX){
            return ret;
        }
        return cnt[u] ? key[u] : upperbound(rs(u), value);
    }

    int prev(int u, int value){
        if (!u || !tot[u]){
            return INT_MIN;
        }
        if (value <= key[u]){
            return prev(ls(u), value);
        }
        int ret = prev(rs(u), value);
        if (ret != INT_MIN){
            return ret;
        }
        return cnt[u] ? key[u] : prev(ls(u), value);
    }

    int rank(int u, int value){
        if (!u){
            return 0;
        }
        if (value > key[u]){
            return tot[ls(u)] + cnt[u] + rank(rs(u), value);
        }
        return rank(ls(u), value);
    }

    int query(int u, int value){
        if (tot[ls(u)] >= value) return query(ls(u), value);
        value -= tot[ls(u)];
        if (cnt[u] >= value) return key[u];
        value -= cnt[u];
        if (tot[rs(u)] >= value) return query(rs(u), value);
        return INT_MAX;
    }
};

ScapegoatTree st;

int main(){
    st.init();
    int n;
    scanf("%d", &n);
    while (n --){
        int op, x;
        scanf("%d%d", &op, &x);
        switch (op) {
            case 1: st.insert(st.root, x); break;
            case 2: st.del(st.root, x); break;
            case 3: printf("%d\n", st.rank(st.root, x) + 1); break;
            case 4: printf("%d\n", st.query(st.root, x)); break;
            case 5: printf("%d\n", st.prev(st.root, x)); break;
            case 6: printf("%d\n", st.upperbound(st.root, x)); break;
        }
    }
    return 0;
}