#include <bits/stdc++.h>

const int N = 1000010;
const int BIT = 20;

char s[N];
int sa[N], id[N], rk[N], oldrk[N << 1], tmp[N], cnt[N], height[N];
int bit[N], st[N][BIT];


void get_sa(char *s, int *sa){
    int n = strlen(s + 1), m = 300;
    for (int i = 1; i <= n; ++ i) ++ cnt[rk[i] = s[i]];
    for (int i = 1; i <= m; ++ i) cnt[i] += cnt[i - 1];
    for (int i = n; i >= 1; -- i) sa[cnt[s[i]] --] = i;
    if (n == 1) rk[1] = 1;
    auto equal = [&](int x, int y, int w){return oldrk[x] == oldrk[y] && oldrk[x + w] == oldrk[y + w];};
    for (int w = 1, p; w < n; w <<= 1, m = p){
        p = 0;
        for (int i = n; i > n - w; -- i) id[++ p] = i;
        for (int i = 1; i <= n; ++ i){
            if (sa[i] > w) id[++ p] = sa[i] - w;
        }
        memset(cnt, 0, sizeof(cnt));
        for (int i = 1; i <= n; ++ i) ++ cnt[tmp[i] = rk[id[i]]];
        for (int i = 1; i <= m; ++ i) cnt[i] += cnt[i - 1];
        for (int i = n; i >= 1; -- i) sa[cnt[tmp[i]] --] = id[i];
        memcpy(oldrk, rk, sizeof(rk));
        p = 0;
        for (int i = 1; i <= n; ++ i){
            rk[sa[i]] = equal(sa[i - 1], sa[i], w) ? p : ++ p;
        }
    }
    for (int i = 1, p = 0; i <= n; ++ i){
        if (p) -- p;
        while (s[i + p] == s[sa[rk[i] - 1] + p]) ++ p;
        height[rk[i]] = p;
    }
    for (int i = 0; i <= n; ++ i){
        st[i][0] = height[i];
    }
    for (int j = 1; j < BIT; ++ j){
        for (int i = 0; i <= n; ++ i){
            int next = i + (1 << (j - 1));
            if (next > n){
                break;
            }
            st[i][j] = std::min(st[i][j - 1], st[next][j - 1]);
        }
    }
}

void init(){
    memset(cnt, 0, sizeof(cnt));
    memset(rk, 0, sizeof(rk));
    memset(st, 0, sizeof(st));
}

int query(int a, int b){
    if (a == b) return -1;
    a = rk[a], b = rk[b];
    if (a > b){
        std::swap(a, b);
    }
    ++ a;
    int diff = b - a + 1;
    int bt = bit[diff];
    return std::min(st[a][bt], st[b - (1 << bt) + 1][bt]);
}
