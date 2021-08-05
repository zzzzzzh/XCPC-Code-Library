#include <bits/stdc++.h>

const int N = 1000010;
using ll = long long;

struct SAM{
    const static int N = 1000010;
    const static int MAXNODE = N << 1;
    const static int DICTSIZE = 26;

    int go[MAXNODE][DICTSIZE], link[MAXNODE], len[MAXNODE];
    int last, sz, root;
    std::vector<int> e[MAXNODE]; // parent tree

    int newnode(){
        ++ sz;
        memset(go[sz], 0, sizeof(go[sz]));
        return sz;
    }

    void init(){
        sz = 0;
        root = last = newnode();
        len[root] = link[root] = 0;
    }

    int split(int p, int q, int ch){
        int clone = newnode();
        memcpy(go[clone], go[q], sizeof(go[q]));
        link[clone] = link[q];
        link[q] = clone;
        len[clone] = len[p] + 1;
        for (int i = p; i && go[i][ch] == q; i = link[i]){
            go[i][ch] = clone;
        }
        return clone;
    }

    void insert(int ch){
        if (go[last][ch]){ // generalized SAM
            int q = go[last][ch];
            last = len[last] + 1 == len[q] ? q : split(last, q, ch);
            return;
        }
        int cur = newnode();
        len[cur] = len[last] + 1;
        int p = last;
        for ( ; p && !go[p][ch]; p = link[p]){
            go[p][ch] = cur;
        }
        if (!p){
            link[cur] = root;
        }
        else{
            int q = go[p][ch];
            link[cur] = len[p] + 1 == len[q] ? q : split(p, q, ch);
        }
        last = cur;
    }

    void build(char *s){ // must init() first
        for (char *p = s; *p != '\0'; ++ p){
            insert(*p - 'a');
        }
        last = root;
    }

    void build_parent_tree(){
        for (int i = root; i <= sz; ++ i){
            e[link[i]].emplace_back(i);
        }
    }
};
