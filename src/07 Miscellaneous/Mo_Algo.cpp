#include <bits/stdc++.h>

const int N = 100010;

struct Query{
    const static int BLOCK = 320;

    int l, r, id;

    bool operator < (const Query &q)const{
        int b1 = l / BLOCK, b2 = q.l / BLOCK;
        if (b1 != b2) return b1 < b2;
        return b1 & 1 ? r < q.r : r > q.r;
    }
};

Query query[N];

int ans;

int main(){
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; ++ i){
        scanf("%d%d", &query[i].l, &query[i].r);
        query[i].id = i;
    }
    std::sort(query, query + n);
    int l = 0, r = 0;
    ans = 1;
    for (int i = 0; i < n; ++ i){
        Query q = query[i];
        while (r < q.r) move_r_r(l, r);
        while (l > q.l) move_l_l(l, r);
        while (r > q.r) move_r_l(l, r);
        while (l < q.l) move_l_r(l, r);
    }
    return 0;
}