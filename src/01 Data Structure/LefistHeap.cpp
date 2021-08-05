#include <bits/stdc++.h>
#define ls(u) (ch[(u)][0])
#define rs(u) (ch[(u)][1])

namespace LH{
    const static int N = 100010;

    int sz;
    int value[N], dis[N];
    int ch[N][2];

    int newnode(int val){
        ++ sz;
        value[sz] = val;
        dis[sz] = 0;
        memset(ch[sz], 0, sizeof(ch[0]));
        return sz;
    }

    void init(){
        sz = 0;
    }

    int merge(int u, int v){
        if (!u || !v) return u ^ v;
        if (value[u] > value[v]){
            std::swap(u, v);
        }
        rs(u) = merge(rs(u), v);
        if (dis[rs(u)] > dis[ls(u)]){
            std::swap(ls(u), rs(u));
        }
        dis[u] = dis[rs(u)] + 1;
        return u;
    }

    struct LefistHeap{
        int root;

        explicit LefistHeap():root(0){}

        void init(int val){
            root = newnode(val);
        }

        int top(){
            return value[root];
        }

        void push(int val){
            int nd = newnode(val);
            root = merge(root, nd);
        }

        void pop(){
            root = merge(ls(root), rs(root));
        }

        void join(LefistHeap lh){
            root = merge(root, lh.root);
        }
    };
}

int main(){

}