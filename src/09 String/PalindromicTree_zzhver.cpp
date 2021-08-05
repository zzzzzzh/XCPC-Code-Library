#include <bits/stdc++.h>

const int N = 1000010;
using ll = long long;

struct PAM{
    static const int N = 1000010;
    static const int SIGMA = 26;

    int go[N][SIGMA], len[N], link[N];
    int s[N]; // start from 1
    int root, even_root, last, sz;
    int cnt[N]; //

    int newnode(){
        ++ sz;
        memset(go[sz], 0, sizeof(go[sz]));
        return sz;
    }

    void init(){
        sz = 0;
        root = newnode(), even_root = newnode();
        s[0] = len[root] = -1;
        last = link[even_root] = link[root] = root;
    }

    int search(int pos, int cur){
        while (s[pos - len[cur] - 1] != s[pos]){
            cur = link[cur];
        }
        return cur;
    }

    void insert(int ch, int pos){ // ch - 'a'
        s[pos] = ch;
        last = search(pos, last);
        if (!go[last][ch]){
            int cur = go[last][ch] = newnode();
            len[cur] = len[last] + 2;
            link[cur] = last == root ? even_root : go[search(pos, link[last])][ch];
        }
        last = go[last][ch];
    }

    void build(char *s){ // should call init() before
        for (char *p = s; *p != '\0'; ++ p){
            insert(*p - 'a', p - s + 1);
            ++ cnt[last]; //
        }
        last = root; // generalized PAM
    }

    void get_cnt(){
        for (int i = sz; i > root; -- i){
            cnt[link[i]] += cnt[i];
        }
    }
};
